#include "draw.h"
#include "GraphicsAPI/graphics_api.h"

VLADLIB_NAMESPACE_USING;

Geom::Mesh TransformConfirmer::ConfirmMesh(const Geom::Mesh& m, const Geom::Transform& t) {
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

    return {points, m.indices};
}

std::vector<glm::vec3> TransformConfirmer::ConfirmPoints(std::vector<glm::vec3> points, const Geom::Transform& t) {
    glm::mat4 tm(1.0f);
    glm::vec3 deltaPivot = t.deltaPivot;

    tm = glm::translate(tm, t.position);

    tm = glm::rotate(tm, glm::radians(t.rotation.x), glm::vec3(1, 0, 0));
    tm = glm::rotate(tm, glm::radians(t.rotation.y), glm::vec3(0, 1, 0));
    tm = glm::rotate(tm, glm::radians(t.rotation.z), glm::vec3(0, 0, 1));

    tm = glm::scale(tm, t.scale);
    tm = glm::translate(tm, -deltaPivot);

    for (uint32_t i = 0; i < points.size(); i++) {
        glm::vec4 point = {points[i], 1.0f};
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
        { 0,          imageHeight },
        { 0,          0           },
        { imageWidth, 0           },
        { imageWidth, imageHeight },
    };

    if (uv->empty())
        uv = &uv_cords;

    static std::vector<Vertex> vertices;
    static std::vector<glm::vec3> points;
    vertices.clear();
    points.clear();

    points = TransformConfirmer::ConfirmPoints(e.mesh->points, *e.transform);

    for (uint32_t i = 0; i < p.size(); i++) {
        float uvx = (*uv)[i % uv->size()].x;
        uvx = (uvx == -1) ? imageWidth : uvx;

        float uvy = (*uv)[i % uv->size()].y;
        uvy = (uvy == -1) ? imageHeight : uvy;

        glm::vec3 uvCord = {
                uvx / ((float) imageWidth),
                1 - uvy / ((float) imageHeight),
                0
        };
        glm::vec4 col = (i < color->size()) ? (*color)[i] : basicColor;
        vertices.push_back({.pos = points[i], .col = col, .uv = uvCord});
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




static GAPI::RenderItem CreateBasicsDrawResources() {
    GAPI::RenderItem item;

    item.vertexArray = GAPI::createVertexArray();
    item.vertexBuffer = GAPI::createVertexBuffer(GAPI::DRAW_TYPE::DYNAMIC, 0, nullptr);
    item.elementBuffer = GAPI::createElementBuffer(GAPI::DRAW_TYPE::DYNAMIC, 0, nullptr);

    GAPI::VertexArrayLayout layout;
    layout.Add<float>(3);
    layout.Add<float>(4);
    layout.Add<float>(3);
    item.vertexArray->AddVertexBuffer(layout, *item.vertexBuffer);
    item.vertexArray->AddElementBuffer(*item.elementBuffer);

    return item;
}



SceneRenderer::SceneRenderer(GAPI::Window* window)
{
    m_Batch.SetMaxBatchSize(10'000);

    m_Item = CreateBasicsDrawResources();

    std::shared_ptr<uint8_t> bitmap(new uint8_t[4], [](const uint8_t* p) { delete[] p; });
    for (uint32_t i = 0; i < 4; i++)
        bitmap.get()[i] = 255;

    m_StandardTex = GAPI::ImageInfo(1, 1, 4, bitmap);

    m_TexManager.RegisterAtlas({.magFilter = GAPI::TEXTURE_PARAM::NEAREST, .minFilter = GAPI::TEXTURE_PARAM::NEAREST});
    m_TexManager.RegisterAtlas({.magFilter = GAPI::TEXTURE_PARAM::LINEAR,  .minFilter = GAPI::TEXTURE_PARAM::LINEAR});

    m_BaseShader.AddSrcFile(
        "resources/shaders/base.vert",
        GAPI::SHADER_TYPE::VERTEX
    );
    m_BaseShader.AddSrcFile(
        "resources/shaders/base.frag",
        GAPI::SHADER_TYPE::FRAGMENT
    );
    m_BaseShader.Compile();

    m_Window = window;
}

void SceneRenderer::StartDraw() {
    m_Renderer->Clear(GAPI::CLEAR_BUFFER_BIT::COLOR | GAPI::CLEAR_BUFFER_BIT::DEPTH);
}

void SceneRenderer::EndDraw() {
    FlushBatch();
    m_Window->SwapDrawingBuffer();

    auto stats = m_Renderer->GetStats();
    // LOGINF("Draw calls: " + std::to_string(stats.drawCalls));
}

void SceneRenderer::DrawEntity(const Geom::Entity& e) {
    const GAPI::ImageInfo* imageInfo = &m_StandardTex;
    GAPI::ShaderProgram* shaderProg = m_BaseShader.GetShaderProgram();
    if (auto* mat = e.material.get()) {
        imageInfo = (mat->image) ? mat->image : imageInfo;
        if (mat->shader && mat->shader->GetShaderProgram())
            shaderProg = mat->shader->GetShaderProgram();
    }

    auto textureInstance = m_TexManager.GetTextureObject(*imageInfo);
    if (textureInstance != m_Item.texture || m_Item.shader != shaderProg) {
        FlushBatch();
    }

    m_Item.texture = textureInstance;
    m_Item.shader = shaderProg;

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

void moveBatchIntoItem(Batch<Vertex>& batch, GAPI::RenderItem& item) {
    item.vertexBuffer->PutData(batch.GetVerticesData(), batch.GetVerticesCapacity());
    item.elementBuffer->PutData(batch.GetIndicesData(), batch.GetIndicesCount());

    batch.Clear();
}

void SceneRenderer::FlushBatch() {
    if (m_Batch.GetVerticesCapacity() == 0) return;

    if (!m_Item.shader) return;
    if (!m_Item.texture) {
        LOGERR("Texture: Texture instance is empty!");
        assert(0);
    }

    moveBatchIntoItem(m_Batch, m_Item);
    m_Item.shader->SetInt("u_Texture", 0);
    m_Item.shader->SetMatrixFloat4("u_MVP", &m_Camera->GetVP()[0][0]);

    m_Renderer->Draw(m_Item);
}

void SceneRenderer::RegisterFrameBaker(const FrameBaker &fm) {
    m_TexManager.RegisterTextureInstance(&fm.image, fm.m_Texture.get());
}

void syncImageWithWindow(GAPI::ImageInfo& im, GAPI::Window& w) {
    if (im.GetWidth() != w.GetWidth() || im.GetHeight() != w.GetHeight()) {
        auto texParam = im.GetTexParams();
        im = GAPI::ImageInfo(w.GetWidth(), w.GetHeight(), 4, nullptr);
        im.SetTexParam(texParam);
    }
}

void SceneRenderer::StartBake(FrameBaker& fm) {
    FlushBatch();

    m_FrameBakers.push({&fm, m_Window->GetViewport()});

    fm.syncTextureWithImage();
    fm.StartBake();
    m_Renderer->Clear(GAPI::CLEAR_BUFFER_BIT::COLOR | GAPI::CLEAR_BUFFER_BIT::DEPTH);

    m_Window->SetViewport({0, 0, (int)fm.image.GetWidth(), (int)fm.image.GetHeight()} , 1);
}

void SceneRenderer::EndBake() {
    FlushBatch();

    auto pair = m_FrameBakers.top();
    pair.first->EndBake();

    m_Window->SetViewport(pair.second);

    m_FrameBakers.pop();
}
