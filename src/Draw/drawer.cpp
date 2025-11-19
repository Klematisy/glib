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

Drawer::Drawer(RendererCore::Window& window) {
    m_FD = std::make_unique<FramebufferDrawer>(window);
    m_BasicTexture = TextureManager::GetBasicTex();

    std::shared_ptr<RendererCore::TextureArray> linearTextureArray = std::make_shared<RendererCore::TextureArray>();
    std::shared_ptr<RendererCore::TextureArray> nearestTextureArray = std::make_shared<RendererCore::TextureArray>();

    initTexArrWithParam(linearTextureArray, GAPI::TEXTURE_PARAM::LINEAR);
    initTexArrWithParam(nearestTextureArray, GAPI::TEXTURE_PARAM::NEAREST);

    m_LinearTexManager.SetTextureArray(linearTextureArray);
    m_NearestTexManager.SetTextureArray(nearestTextureArray);

    m_BasicShader = std::make_shared<Shader>();
    m_BasicFontShader = std::make_shared<Shader>();

    m_BasicShader->AddSrcFiles("resources/shaders/base_shader.glsl");
    m_BasicFontShader->AddSrcFiles("resources/shaders/font.glsl");

    m_BasicShader->Compile();
    m_BasicFontShader->Compile();
}

void Drawer::DrawMesh(const Geom::Mesh& mesh, const Color& color, const Texture* texture, Shader* shader) {
    const Texture* tex = &m_BasicTexture;
    Shader* sh = m_BasicShader.get();

    if (texture)
        tex = texture;
    if (shader)
        sh = shader;

    if (sh != m_FD->GetBoundShader() || &m_NearestTexManager != m_FD->GetBoundTexManager()) {
        m_FD->FlushBuffer();

        m_FD->UseShader(sh);
        m_FD->UseTextureManager(&m_NearestTexManager);
    }

    m_FD->BatchMesh(mesh, color, tex);
}

void Drawer::DrawText(const Geom::Text2D& text2D, const Color& color, Shader* shader) {
    Shader* sh = m_BasicFontShader.get();

    if (shader) sh = shader;

    if (sh != m_FD->GetBoundShader() || &m_LinearTexManager != m_FD->GetBoundTexManager()) {
        m_FD->FlushBuffer();

        m_FD->UseShader(sh);
        m_FD->UseTextureManager(&m_LinearTexManager);
    }

    m_FD->BatchText(text2D, color);
}

void Drawer::Start() {
    m_FD->Start();
}

void Drawer::End() {
    m_FD->End();
}
