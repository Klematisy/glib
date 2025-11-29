#include "drawer.h"

GLIB_NAMESPACE_USING;

static glm::mat4 GetProjMatrix(const glib::Rectangle& rect) {
    return glm::ortho(rect.x, rect.width,
                      rect.y, rect.height,
                      -100.0f, 100.0f);
}

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
      : m_StandardCamera(&window), m_Window(&window)
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
    m_MainFrameBaker = std::make_shared<FrameBaker>(&window);

    m_BufferDrawer.UseBuffer(&m_MainFrameBuffer->GetBuffer());
    m_BufferDrawer.UseTextureManager(&m_LinearTexManager);
}

void Drawer::Start() {
    m_MainFrameBuffer->BeginRenderCatch();
    m_BufferDrawer.Start();

    m_BufferDrawer.SetProjMatrix(
        GetProjMatrix({0, (float) m_Window->GetHeight(), (float) m_Window->GetWidth(), 0})
    );

    m_BufferDrawer.SetViewMatrix(m_Camera->GetView());
}

void Drawer::End() {
    m_BufferDrawer.End();
    m_MainFrameBuffer->EndRenderCatch();
}

void Drawer::BeginBake(FrameBaker* baker, const RendererCore::Rectangle& renderViewport) {
    m_BufferDrawer.FlushBuffer();
    m_FrameBakers.push(baker);

    baker->BeginRenderCatch(renderViewport);
    RendererCore::Renderer::Clear();
    m_Window->ChangeViewport({0, 0, m_Window->GetWidth(), m_Window->GetHeight()});

    glm::mat4 proj(1.0f);

    const auto* r = &renderViewport;
    proj = GetProjMatrix({(float) r->x,
                          (float) r->y,
                          (float) r->x + r->width,
                          (float) r->y + r->height});

    m_BufferDrawer.SetProjMatrix(proj);
    m_BufferDrawer.SetViewMatrix(m_StandardCamera.GetView());
}

void Drawer::EndBake() {
    m_BufferDrawer.FlushBuffer();

    auto* baker = m_FrameBakers.top();
    baker->EndRenderCatch();
    m_FrameBakers.pop();

    glm::mat4 proj(1.0f);

    if (!m_FrameBakers.empty()) {
        baker = m_FrameBakers.top();
        auto b = baker->GetBakeField();
        baker->BeginRenderCatch(b);
        proj = GetProjMatrix({(float) b.x,
                              (float) b.y,
                              (float) b.x + b.width,
                              (float) b.y + b.height});
        m_BufferDrawer.SetViewMatrix(m_StandardCamera.GetView());
    } else {
        proj = GetProjMatrix({0, (float) m_Window->GetHeight(),
                              (float) m_Window->GetWidth(), 0});
        if (m_Camera)
            m_BufferDrawer.SetViewMatrix(m_Camera->GetView());
    }

    m_BufferDrawer.SetProjMatrix(proj);
    m_Window->ChangeViewport({0, 0, m_Window->GetWidth(), m_Window->GetHeight()});
}

void Drawer::DrawBakedTexture(const Geom::Mesh& mesh, FrameBaker& fm, Shader* shader) {
    shader = (shader) ? shader : m_BasicShader.get();

    if (m_FrameBakers.empty() && m_Camera)
        m_BufferDrawer.SetViewMatrix(m_Camera->GetView());

    DrawMesh(mesh, *fm.GetTextureManager(), &fm.GetRenderTexture(), shader);
    m_BufferDrawer.FlushBuffer();

    if (!m_FrameBakers.empty())
        m_BufferDrawer.SetViewMatrix(m_StandardCamera.GetView());
}

void Drawer::DrawMesh(const Geom::Mesh& mesh, const Texture* texture, Shader* shader) {
    shader = (shader) ? shader : m_BasicShader.get();
    texture = (texture) ? texture : &m_BasicTexture;

    DrawMesh(mesh, m_NearestTexManager, texture, shader);
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

    BeginBake(m_FontBaker.get(), {0, 0, (int) textSize.x, (int) textSize.y});
    m_BufferDrawer.UseShader(m_BasicFontShader.get());
    m_BufferDrawer.UseTextureManager(&m_LinearTexManager);
    m_BufferDrawer.BatchText(text2D);
    m_BufferDrawer.FlushBuffer();
    EndBake();

    auto mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    mesh.SetPosition(text2D.ReadMesh()->GetPosition());
    mesh.SetRotation(text2D.ReadMesh()->GetRotation());
    mesh.SetColor(text2D.ReadMesh()->GetColor());
    mesh.SetScale({textSize.x,
                   textSize.y,
                   0.0f});

    DrawBakedTexture(mesh, *m_FontBaker, shader);
}

void Drawer::DrawMesh(const Geom::Mesh& mesh, TextureManager& tm, const Texture* texture, Shader* shader) {
    if (shader != m_BufferDrawer.GetBoundShader() ||
        &tm != m_BufferDrawer.GetBoundTexManager())
    {
        m_BufferDrawer.FlushBuffer();
    }

    m_BufferDrawer.UseShader(shader);
    m_BufferDrawer.UseTextureManager(&tm);
    m_BufferDrawer.BatchMesh(mesh, texture);
}

const Camera* Drawer::GetCamera() const {
    return m_Camera;
}

void Drawer::SetCamera(Camera* camera) {
    m_Camera = camera;
}
