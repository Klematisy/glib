#include "drawer.h"

GLIB_NAMESPACE_USING;

static void initTexArrWithParam(std::shared_ptr<RendererCore::TextureArray>& texArr, GAPI::TEXTURE_PARAM texParam) {
    texArr->Bind();
    texArr->SetWidth(TexInfo::WIDTH_MAX_SIZE);
    texArr->SetHeight(TexInfo::HEIGHT_MAX_SIZE);

    texArr->SetLayersCount(16);

    texArr->Parameteri(GAPI::TEXTURE_PROPERTY::MIN_FILTER, texParam);
    texArr->Parameteri(GAPI::TEXTURE_PROPERTY::MAG_FILTER, texParam);
    texArr->Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_S, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);
    texArr->Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_T, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);

    texArr->AllocateTexture();
}

static void InitShader(std::shared_ptr<Shader>& shader, const char* str) {
    shader = std::make_shared<Shader>();
    shader->AddSrcFiles(str);
    shader->Compile();
}

Texture tex(3000, 3000, 4);
Drawer::Drawer(RendererCore::Window& window)
      : m_Window(&window)
{
    std::shared_ptr<RendererCore::TextureArray> linearTextureArray = std::make_shared<RendererCore::TextureArray>();
    std::shared_ptr<RendererCore::TextureArray> nearestTextureArray = std::make_shared<RendererCore::TextureArray>();

    initTexArrWithParam(linearTextureArray, GAPI::TEXTURE_PARAM::LINEAR);
    initTexArrWithParam(nearestTextureArray, GAPI::TEXTURE_PARAM::NEAREST);

    m_LinearTexManager.SetTextureArray(linearTextureArray);
    m_NearestTexManager.SetTextureArray(nearestTextureArray);

    InitShader(m_BasicShader, "resources/shaders/base_shader.glsl");
    InitShader(m_BasicFontShader, "resources/shaders/font.glsl");

    m_MainFrameBuffer = std::make_shared<Framebuffer>(&window);

    m_FontBaker = std::make_shared<FrameBaker>(&window);
    m_FontBaker->SetRenderTexture(m_NearestTexManager, tex);

    m_BufferDrawer.UseBuffer(&m_MainFrameBuffer->GetBuffer());
    m_BufferDrawer.UseTextureManager(&m_LinearTexManager);
}

void Drawer::UseBaker(std::shared_ptr<FrameBaker> baker) {
    m_FrameBakers.push(baker);

    baker->BeginRenderCatch();
    m_BufferDrawer.SetProjMatrix(baker->GetProjMatrix());
}

void Drawer::UnUseBaker() {
    auto& baker = m_FrameBakers.top();
    baker->EndRenderCatch();
    m_FrameBakers.pop();

    if (!m_FrameBakers.empty()) {
        baker = m_FrameBakers.top();
        baker->BeginRenderCatch();
        m_BufferDrawer.SetProjMatrix(baker->GetProjMatrix());
    } else {
        m_BufferDrawer.SetProjMatrix(m_MainFrameBuffer->GetProjMatrix());
    }
}

void Drawer::DrawMesh(const Geom::Mesh& mesh, const Texture* texture, Shader* shader) {
    shader = (shader) ? shader : m_BasicShader.get();
    texture = (texture) ? texture : &m_BasicTexture;

    if (shader != m_BufferDrawer.GetBoundShader() ||
        &m_NearestTexManager != m_BufferDrawer.GetBoundTexManager())
    {
        m_BufferDrawer.FlushBuffer();
    }

    m_BufferDrawer.UseShader(shader);
    m_BufferDrawer.UseTextureManager(&m_NearestTexManager);
    m_BufferDrawer.BatchMesh(mesh, texture);
}

void Drawer::DrawText(const Geom::Text2D& text2D, Shader* shader) {
    shader = (shader) ? shader : m_BasicShader.get();

    if (m_BasicFontShader.get() != m_BufferDrawer.GetBoundShader() ||
        shader != m_BufferDrawer.GetBoundShader() ||
        &m_LinearTexManager != m_BufferDrawer.GetBoundTexManager())
    {
        m_BufferDrawer.FlushBuffer();
    }

    m_BufferDrawer.UseShader(m_BasicFontShader.get());
    m_BufferDrawer.UseTextureManager(&m_LinearTexManager);

    UseBaker(m_FontBaker);
    m_BufferDrawer.BatchText(text2D);
    m_BufferDrawer.FlushBuffer();
    UnUseBaker();

    auto mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    mesh.SetColor(text2D.ReadMesh()->GetColor());
    mesh.SetScale({600.0f, 600.0f, 1.0f});
    DrawMesh(mesh, &tex, shader);
}

void Drawer::Start() {
    m_MainFrameBuffer->BeginRenderCatch();
    m_BufferDrawer.SetProjMatrix(m_MainFrameBuffer->GetProjMatrix());
    m_BufferDrawer.Start();
}

void Drawer::End() {
    m_BufferDrawer.End();
    m_MainFrameBuffer->EndRenderCatch();
}