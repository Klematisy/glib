#include "draw.h"

GLIB_NAMESPACE_USING;

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
    m_Camera = cam;
}

void Draw::FlushBatch() {
    if (!m_LastShaderProgram) return;

    m_GB.vertexBuffer->PutData(m_Batch.GetVerticesSize() * sizeof(Vertex), m_Batch.GetVerticesData());
    m_GB.elementBuffer->PutData(m_Batch.GetIndicesSize(), m_Batch.GetIndicesData());

    m_Batch.Clear();

    m_TexManager.Bind(m_LastTexParams);
    m_LastShaderProgram->Bind();
    m_LastShaderProgram->SetUniform1i("u_Texture", 0);
    m_LastShaderProgram->SetUniformMatrix4fv("u_MVP", &m_Camera->GetVP()[0][0]);

    m_Renderer.Draw(m_GB, *m_LastShaderProgram);
}
