#include "draw.h"

GLIB_NAMESPACE_USING;

std::vector<Vertex> EntityToVerticesEvaluator::Convert(const Geom::Entity& e, const TexInfoConstRef& texInfo) {
    glm::mat4 tm(1.0f);
    if (e.transform) {
        const auto& trans = *e.transform;
        glm::vec3 deltaPivot = trans.deltaPivot;

        tm = glm::translate(tm, trans.position);

        tm = glm::rotate(tm, glm::radians(trans.rotation.x), glm::vec3(1, 0, 0));
        tm = glm::rotate(tm, glm::radians(trans.rotation.y), glm::vec3(0, 1, 0));
        tm = glm::rotate(tm, glm::radians(trans.rotation.z), glm::vec3(0, 0, 1));

        tm = glm::scale(tm, trans.scale);
        tm = glm::translate(tm, -deltaPivot);
    }

    const auto& p = e.mesh->points;
    const auto* uv = &e.material->uvCoordinates;
    const auto* color = &e.material->colors;

    glm::vec4 basicColor(1.0f);
    std::vector<glm::vec2> uv_cords {
            { 0, 0 },
            { 0, 1 },
            { 1, 1 },
            { 1, 0 },
    };
    if (uv->empty())
        uv = &uv_cords;

    static std::vector<Vertex> vertices;
    vertices.clear();

    for (uint32_t i = 0; i < p.size(); i++) {
        glm::vec4 point = {p[i], 1.0f};
        glm::vec3 uvCord = {
                (*uv)[i % uv->size()].x / ((float) texInfo->GetWidth()),
                (*uv)[i % uv->size()].y / ((float) texInfo->GetHeight()),
                0
        };
        glm::vec4 col = (i < color->size()) ? (*color)[i] : basicColor;
        vertices.push_back({.pos = tm * point, .color = col, .uv = uvCord});
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

    m_StandardTex = rc::ImageInfo(1, 1, 4, bitmap);

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
    auto imageInfo = (e.material->image) ? e.material->image : &m_StandardTex;
    auto shaderProg = (e.material->shader.get()) ? e.material->shader.get() : m_BaseShader.GetShaderProgram().get();

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

void Draw::FlushBatch() {
    if (!m_LastShaderProgram) return;

    m_GB.vertexBuffer->PutData(m_Batch.GetVerticesSize() * sizeof(Vertex), m_Batch.GetVerticesData());
    m_GB.elementBuffer->PutData(m_Batch.GetIndicesSize(), m_Batch.GetIndicesData());

    m_Batch.Clear();

    m_TexManager.Bind(m_LastTexParams);
    m_LastShaderProgram->SetInt("u_Texture", 0);
    if (m_FrameBakers.empty()) {
        m_LastShaderProgram->SetMatrixFloat4("u_MVP", &m_Camera->GetVP()[0][0]);
    } else
        m_LastShaderProgram->SetMatrixFloat4("u_MVP", &m_Camera->GetProject()[0][0]);

    m_Renderer.Draw(m_GB, *m_LastShaderProgram);
}

void Draw::TieImageAndFrameBuffer(RendererCore::ImageInfo& image, FrameBaker& fm) {
    auto textureParams = image.GetTexParams();
    image = RendererCore::ImageInfo(3000, 3000, 4, nullptr);
    image.SetTexParam(textureParams);
    auto texInfo = m_TexManager.GetTextureInformation(image);

    RendererCore::AttachTextureArrayToFramebuffer(fm.GetFrameBuffer(),
                                                  *m_TexManager.GetAtlas(image.GetTexParams()).GetTextureObject(),
                                                  GAPI::ATTACHMENT::COLOR0, texInfo->GetSlot());
}

void Draw::StartBake(FrameBaker& fm) {
    FlushBatch();
    fm.StartBake();
    m_FrameBakers.push(&fm);
    m_Window->ChangeViewport({0, 0, 3000, 3000}, 1);
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
