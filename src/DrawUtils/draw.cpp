#include "draw.h"

GLIB_NAMESPACE_USING;

std::vector<glm::vec3> TransformConfirmer::Confirm(const Geom::Mesh& m, const Geom::Transform& t) {
    glm::mat4 tm(1.0f);
    glm::vec3 deltaPivot = t.deltaPivot;

    tm = glm::translate(tm, t.position);

    tm = glm::rotate(tm, glm::radians(t.rotation.x), glm::vec3(1, 0, 0));
    tm = glm::rotate(tm, glm::radians(t.rotation.y), glm::vec3(0, 1, 0));
    tm = glm::rotate(tm, glm::radians(t.rotation.z), glm::vec3(0, 0, 1));

    tm = glm::scale(tm, t.scale);
    tm = glm::translate(tm, -deltaPivot);

    std::vector<glm::vec3> points(m.points.size());
    for (uint32_t i = 0; i < points.size(); i++) {
        glm::vec4 point = {m.points[i], 1.0f};
        points[i] = tm * point;
    }

    return points;
}

std::vector<Vertex> EntityToVerticesEvaluator::Convert(const Geom::Entity& e, const TexInfoConstRef& texInfo) {
    if (!e.mesh) return {};

    const auto& p = e.mesh->points;
    const auto* uv = &e.material->uvCoordinates;
    const auto* color = &e.material->colors;

    glm::vec4 basicColor(1.0f);
    std::vector<glm::vec2> uv_cords {
            { 0,                   0                    },
            { 0,                   texInfo->GetHeight() },
            { texInfo->GetWidth(), texInfo->GetHeight() },
            { texInfo->GetWidth(), 0                    },
    };
    if (uv->empty())
        uv = &uv_cords;

    static std::vector<Vertex> vertices;
    static std::vector<glm::vec3> points;
    vertices.clear();
    points.clear();

    points = TransformConfirmer::Confirm(*e.mesh, *e.transform);

    for (uint32_t i = 0; i < p.size(); i++) {
        glm::vec3 uvCord = {
                (*uv)[i % uv->size()].x / ((float) texInfo->GetWidth()),
                (*uv)[i % uv->size()].y / ((float) texInfo->GetHeight()),
                0
        };
        glm::vec4 col = (i < color->size()) ? (*color)[i] : basicColor;
        vertices.push_back({.pos = points[i], .color = col, .uv = uvCord});
    }

    for (auto& it : vertices) {
        it.uv.x  = (it.uv.x == 1) ? 1.0f : std::fmodf(it.uv.x, 1);
        it.uv.x *= texInfo->GetRectangle().width;
        it.uv.x += texInfo->GetRectangle().x;

        it.uv.y  = (it.uv.y == 1) ? 1.0f : std::fmodf(it.uv.y, 1);
        it.uv.y *= texInfo->GetRectangle().height;
        it.uv.y += texInfo->GetRectangle().y;

        it.uv.z = texInfo->GetSlot();
    }

    return vertices;
}




static RendererCore::GraphicsBuffer CreateDrawBasicsResources() {
    RendererCore::GraphicsBuffer db;

    db.vertexArray = std::make_shared<RendererCore::VertexArray>();
    db.vertexBuffer = std::make_shared<RendererCore::VertexBuffer>(GAPI::DRAW_TYPE::DYNAMIC, 0, nullptr);
    db.elementBuffer = std::make_shared<RendererCore::ElementBuffer>(GAPI::DRAW_TYPE::DYNAMIC, 0, nullptr);

    RendererCore::VertexArrayLayout layout;
    layout.Add<float>(3);
    layout.Add<float>(4);
    layout.Add<float>(3);
    db.vertexArray->AddBuffer(layout, *db.vertexBuffer);

    db.vertexArray->UnBind();
    db.vertexBuffer->UnBind();
    db.elementBuffer->UnBind();

    return db;
}

Draw::Draw(RendererCore::Window* window)
    : m_GB(CreateDrawBasicsResources())
{
    m_Batch.SetMaxBatchSize(10'000);

    std::shared_ptr<uint8_t> bitmap(new uint8_t[4], [](const uint8_t* p) { delete[] p; });
    for (uint32_t i = 0; i < 4; i++)
        bitmap.get()[i] = 255;

    m_StandardTex = RendererCore::ImageInfo(1, 1, 4, bitmap);

    m_TexManager.RegisterAtlas({.magFilter = GAPI::TEXTURE_PARAM::NEAREST, .minFilter = GAPI::TEXTURE_PARAM::NEAREST});
    m_TexManager.RegisterAtlas({.magFilter = GAPI::TEXTURE_PARAM::LINEAR,  .minFilter = GAPI::TEXTURE_PARAM::LINEAR});

    m_Renderer.SetRendererType(GAPI::RENDERER_TYPE::TRIANGLES);

    m_BaseShader.AddSrcFiles("resources/shaders/base_shader.glsl");
    m_BaseShader.Compile();

    m_Window = window;
}

void Draw::StartDraw() {
    m_Renderer.Clear();
}

void Draw::EndDraw() {
    FlushBatch();
    m_Window->SwapDrawingBuffer();
}

void Draw::DrawEntity(const Geom::Entity& e) {
    const RendererCore::ImageInfo* imageInfo = nullptr;
    RendererCore::ShaderProgram* shaderProg = nullptr;
    if (e.material) {
        imageInfo = (e.material->image) ? e.material->image : &m_StandardTex;
        shaderProg = (e.material->shader.get()) ? e.material->shader.get() : m_BaseShader.GetShaderProgram().get();
    } else {
        imageInfo = &m_StandardTex;
        shaderProg = m_BaseShader.GetShaderProgram().get();
    }

    if (m_LastTexParams != imageInfo->GetTexParams() || m_LastShaderProgram != shaderProg) {
        FlushBatch();
    }

    m_LastShaderProgram = shaderProg;
    m_LastTexParams = imageInfo->GetTexParams();

    auto texInfo = m_TexManager.GetTextureInformation(*imageInfo);
    auto vertices = EntityToVerticesEvaluator::Convert(e, texInfo);

    m_Batch.AddVertices(vertices.data(), vertices.size());
    m_Batch.AddIndices(e.mesh->indices.data(), e.mesh->indices.size());
}

void Draw::UseCamera(Camera* cam) {
    FlushBatch();
    m_Camera = cam;
}

Camera* Draw::GetCamera() const {
    return m_Camera;
}

void Draw::FlushBatch() {
    if (!m_LastShaderProgram) return;

    m_GB.vertexBuffer->PutData(m_Batch.GetVerticesSize() * sizeof(Vertex), m_Batch.GetVerticesData());
    m_GB.elementBuffer->PutData(m_Batch.GetIndicesSize(), m_Batch.GetIndicesData());

    m_Batch.Clear();

    m_TexManager.Bind(m_LastTexParams);
    m_LastShaderProgram->SetInt("u_Texture", 0);
    m_LastShaderProgram->SetMatrixFloat4("u_MVP", &m_Camera->GetVP()[0][0]);

    m_Renderer.Draw(m_GB, *m_LastShaderProgram);
}

void Draw::RegisterFrameBuffer(FrameBaker& fm) {
    auto& image = fm.GetImage();
    auto texInfo = m_TexManager.GetTextureInformation(image);

    RendererCore::AttachTextureArrayToFramebuffer(fm.GetFrameBuffer(),
                                                  *m_TexManager.GetAtlas(image.GetTexParams()).GetTextureObject(),
                                                  GAPI::INTERNAL_FORMAT::COLOR_ATTACHMENT0, texInfo->GetSlot());
}

void Draw::StartBake(FrameBaker& fm) {
    FlushBatch();
    fm.StartBake();
    m_FrameBakers.push(&fm);
    m_Window->ChangeViewport({0, 0, TexArrElInfo::WIDTH_MAX_SIZE, TexArrElInfo::HEIGHT_MAX_SIZE}, 1);
    m_Renderer.Clear();
}

void Draw::EndBake() {
    FlushBatch();
    m_FrameBakers.top()->EndBake();
    m_FrameBakers.pop();
    m_Window->ChangeViewport({0, 0,
                              m_Window->GetWidth(),
                              m_Window->GetHeight()});
}