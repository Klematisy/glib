#include "draw.h"

VLADLIB_NAMESPACE_USING;

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

    TexInfo t;
    if (texInfo.IsValid())
        t = *texInfo;
    else
        t = TexInfo::SimpleTex();

    const auto& p = e.mesh->points;
    const auto* uv = &e.material->uvCoordinates;
    const auto* color = &e.material->colors;

    uint32_t imageWidth = 1;
    uint32_t imageHeight = 1;
    if (e.material) {
        if (auto i = e.material->image) {
            imageWidth = i->GetWidth();
            imageHeight = i->GetHeight();
        }
    }

    glm::vec4 basicColor(1.0f);
    std::vector<glm::vec2> uv_cords {
        { 0,          0           },
        { 0,          imageHeight },
        { imageWidth, imageHeight },
        { imageWidth, 0           },
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
                (*uv)[i % uv->size()].x / ((float) imageWidth),
                (*uv)[i % uv->size()].y / ((float) imageHeight),
                0
        };
        glm::vec4 col = (i < color->size()) ? (*color)[i] : basicColor;
        vertices.push_back({.pos = points[i], .color = col, .uv = uvCord});
    }

    for (auto& it : vertices) {
        it.uv.x  = (it.uv.x == 1) ? 1.0f : std::fmodf(it.uv.x, 1);
        it.uv.x *= t.atlasBounds.width;
        it.uv.x += t.atlasBounds.x;

        it.uv.y  = (it.uv.y == 1) ? 1.0f : std::fmodf(it.uv.y, 1);
        it.uv.y *= t.atlasBounds.height;
        it.uv.y += t.atlasBounds.y;

        it.uv.z = t.atlasSlot;
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

SceneRenderer::SceneRenderer(RendererCore::Window* window)
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

void SceneRenderer::StartDraw() {
    m_Renderer.Clear();
}

void SceneRenderer::EndDraw() {
    FlushBatch();
    m_Window->SwapDrawingBuffer();
}

void SceneRenderer::DrawEntity(const Geom::Entity& e) {
    const RendererCore::ImageInfo* imageInfo = nullptr;
    RendererCore::ShaderProgram* shaderProg = nullptr;
    if (e.material) {
        imageInfo = (e.material->image) ? e.material->image : &m_StandardTex;
        shaderProg = m_BaseShader.GetShaderProgram();
        if (e.material->shader) {
            if (e.material->shader->GetShaderProgram())
                shaderProg = e.material->shader->GetShaderProgram();
        }
    } else {
        imageInfo = &m_StandardTex;
        shaderProg = m_BaseShader.GetShaderProgram();
    }

    auto textureInstance = m_TexManager.GetTextureObject(*imageInfo);
    if (textureInstance != m_TextureInstance || m_LastShaderProgram != shaderProg) {
        FlushBatch();
    }

    m_TextureInstance = textureInstance;
    m_LastShaderProgram = shaderProg;

    auto texInfo = m_TexManager.GetTextureInformation(*imageInfo);
    auto vertices = EntityToVerticesEvaluator::Convert(e, texInfo);

    m_Batch.AddVertices(vertices.data(), vertices.size());
    m_Batch.AddIndices(e.mesh->indices.data(), e.mesh->indices.size());
}

void SceneRenderer::UseCamera(Camera* cam) {
    FlushBatch();
    m_Camera = cam;
}

Camera* SceneRenderer::GetCamera() const {
    return m_Camera;
}

void SceneRenderer::FlushBatch() {
    if (!m_LastShaderProgram) return;
    if (!m_TextureInstance) {
        LOGERR("Texture: Texture instance is empty!");
        assert(0);
    }

    m_GB.vertexBuffer->PutData(m_Batch.GetVerticesSize() * sizeof(Vertex), m_Batch.GetVerticesData());
    m_GB.elementBuffer->PutData(m_Batch.GetIndicesSize(), m_Batch.GetIndicesData());

    m_Batch.Clear();

    m_TextureInstance->Bind();
    m_LastShaderProgram->SetInt("u_Texture", 0);
    m_LastShaderProgram->SetMatrixFloat4("u_MVP", &m_Camera->GetVP()[0][0]);

    m_Renderer.Draw(m_GB, *m_LastShaderProgram);
}

void SceneRenderer::RegisterFrameBaker(const FrameBaker &fm) {
    m_TexManager.RegisterTextureInstance(&fm.m_Image, &fm.m_Texture);
}

void syncImageWithWindow(RendererCore::ImageInfo& im, RendererCore::Window& w) {
    if (im.GetWidth() != w.GetWidth() || im.GetHeight() != w.GetHeight()) {
        auto texParam = im.GetTexParams();
        im = RendererCore::ImageInfo(w.GetWidth(), w.GetHeight(), 4, nullptr);
        im.SetTexParam(texParam);
    }
}

void SceneRenderer::StartBake(FrameBaker& fm) {
    FlushBatch();

    syncImageWithWindow(fm.m_Image, *m_Window);
    fm.syncTextureWithImage();

    fm.StartBake();
    m_Window->ChangeViewport({0, 0, m_Window->GetWidth(), m_Window->GetHeight()}, 1);

    m_FrameBakers.push(&fm);
    m_Renderer.Clear();
}

void SceneRenderer::EndBake() {
    FlushBatch();

    m_FrameBakers.top()->EndBake();
    m_FrameBakers.pop();

    m_Window->ChangeViewport({0, 0, m_Window->GetWidth(), m_Window->GetHeight()});
}
