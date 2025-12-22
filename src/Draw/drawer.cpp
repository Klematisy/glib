#include "drawer.h"

GLIB_NAMESPACE_USING;

static void initTexArrWithParam(std::shared_ptr<RendererCore::TextureArray>& texArr, GAPI::TEXTURE_PARAM texParam) {
    texArr->Bind();
    texArr->SetWidth(TexArrElInfo::WIDTH_MAX_SIZE);
    texArr->SetHeight(TexArrElInfo::HEIGHT_MAX_SIZE);

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

Drawer::Drawer(RendererCore::Window& window)
      : m_Window(&window)
{
    m_StandardOCamera = std::make_unique<OrthographicCamera>(&window);
    m_StandardPCamera = std::make_unique<PerspectiveCamera>(&window);

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
    m_MainFrameBaker = std::make_shared<FrameBaker>(&window);

    m_BufferDrawer.UseBuffer(&m_MainFrameBuffer->GetBuffer());
    m_BufferDrawer.UseTextureManager(&m_LinearTexManager);
}

void Drawer::Start() {
    m_MainFrameBuffer->BeginRenderCatch();
    m_BufferDrawer.Start();

    if (m_Camera) m_BufferDrawer.UseCamera(m_Camera);
    else          m_BufferDrawer.UseCamera(m_StandardOCamera.get());
}

void Drawer::End() {
    m_BufferDrawer.End();
    m_MainFrameBuffer->EndRenderCatch();
}

void Drawer::BeginBake(FrameBaker* baker) {
    m_BufferDrawer.FlushBuffer();
    m_FrameBakers.push(baker);

    baker->BeginRenderCatch();
    RendererCore::Renderer::Clear();
    m_Window->ChangeViewport({0, 0, m_Window->GetWidth(), m_Window->GetHeight()});
}

void Drawer::EndBake() {
    m_BufferDrawer.FlushBuffer();

    auto* baker = m_FrameBakers.top();
    baker->EndRenderCatch();
    m_FrameBakers.pop();

    if (!m_FrameBakers.empty())
        m_FrameBakers.top()->BeginRenderCatch();

    if (m_Camera) m_BufferDrawer.UseCamera(m_Camera);
    else          m_BufferDrawer.UseCamera(m_StandardOCamera.get());

    m_Window->ChangeViewport({0, 0, m_Window->GetWidth(), m_Window->GetHeight()});
}

void Drawer::DrawBakedTexture(const Geom::Mesh& mesh, FrameBaker& fm, Shader* shader) {
    shader = (shader) ? shader : m_BasicShader.get();
    DrawMeshWithTextureManager(mesh, &fm.GetRenderTexture(), fm.GetTextureManager().get(), shader);
    m_BufferDrawer.FlushBuffer();
}

void Drawer::DrawMesh(const Geom::Mesh& mesh, const Texture* texture, Shader* shader, GAPI::TEXTURE_PARAM textureParam) {
    DrawMeshWithTextureManager(mesh,
                               ((texture) ? texture : &m_BasicTexture),
                               (textureParam == GAPI::TEXTURE_PARAM::NEAREST) ? &m_NearestTexManager : &m_LinearTexManager,
                               ((shader) ? shader : m_BasicShader.get()));
}

void Drawer::DrawMeshWithTextureManager(const Geom::Mesh &mesh,
                                        const Texture *texture,
                                        TextureManager *tm,
                                        Shader *shader)
{
    if (shader != m_BufferDrawer.GetBoundShader() ||
        tm != m_BufferDrawer.GetBoundTexManager())
    {
        m_BufferDrawer.FlushBuffer();
    }

    m_BufferDrawer.UseShader(shader);
    m_BufferDrawer.UseTextureManager(tm);
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

    const auto& textSize = text2D.GetTextScreenSize();

    m_StandardOCamera->SetRenderRange(0, (float) textSize.x, 0, (float) textSize.y, -1000.0f, 1000.0f);
    m_BufferDrawer.UseCamera(m_StandardOCamera.get());

    BeginBake(m_FontBaker.get());
    m_BufferDrawer.UseShader(m_BasicFontShader.get());
    m_BufferDrawer.UseTextureManager(&m_LinearTexManager);
    m_BufferDrawer.BatchText(text2D);
    m_BufferDrawer.FlushBuffer();
    EndBake();

    m_BufferDrawer.UseCamera(m_Camera);
    auto mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    mesh.SetPosition(text2D.ReadMesh()->GetPosition());
    mesh.SetRotation(text2D.ReadMesh()->GetRotation());
    mesh.SetColor(text2D.ReadMesh()->GetColor());
    mesh.SetScale({textSize.x,
                   textSize.y,
                   0.0f});

    DrawBakedTexture(mesh, *m_FontBaker, shader);
}



const Camera* Drawer::GetCamera() const {
    return m_Camera;
}

void Drawer::SetCamera(Camera* camera) {
    m_Camera = camera;
    m_BufferDrawer.UseCamera(camera);
}
