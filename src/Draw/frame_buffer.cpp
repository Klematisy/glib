#include "frame_buffer.h"

template<class T>
using sptr = std::shared_ptr<T>;

using namespace glib;
namespace rc = RendererCore;

static void initTexArrWithParam(sptr<rc::TextureArray>& texArr, GAPI::TEXTURE_PARAM texParam) {
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

Framebuffer::Framebuffer(rc::Window* window) {
    m_ContentsBuffer = CreateDrawBasicsResources();
    m_Window = window;
}

void Framebuffer::BeginRenderCatch() {
    auto* w = m_Window;

    rc::Renderer::Clear();
}

void Framebuffer::EndRenderCatch() {
    m_Window->SwapDrawingBuffer();
}

DrawBuffer& Framebuffer::GetBuffer() { return m_ContentsBuffer; }

glm::mat4 Framebuffer::GetProjMatrix() {
    return glm::ortho(0.0f, (float) m_Window->GetWidth(),
                      (float) m_Window->GetHeight(), 0.0f,
                      -100.0f, 100.0f);
}

FrameBaker::FrameBaker(rc::Window* window)
        : Framebuffer(window)
{
    sptr<rc::TextureArray> textureArray = std::make_shared<rc::TextureArray>();
    textureArray->SetWidth(3000);
    textureArray->SetHeight(3000);
    textureArray->SetLayersCount(1);
    initTexArrWithParam(textureArray, GAPI::TEXTURE_PARAM::NEAREST);
    textureArray->AllocateTexture();

    m_TexManager = std::make_shared<TextureManager>();
    m_TexManager->SetTextureArray(textureArray);

    m_ContentsBuffer.vertexArray->Bind();

    m_FB.Bind();
    m_RB.Bind();

    m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
    rc::AttachFramebufferToRenderbuffer(m_FB, m_RB, INTERNAL_FORMAT::DEPTH24_STENCIL8);

    m_FB.UnBind();
    m_RB.UnBind();
}

void FrameBaker::UpdateData() {
    if (m_Width != m_Texture->GetWidth() || m_Height != m_Texture->GetHeight()) {
        m_TexManager->Clear();

        m_TexManager->GetTexInfo(m_Texture);
        m_Width = m_Texture->GetWidth();
        m_Height = m_Texture->GetHeight();
    }

    if (m_Width != m_RB.GetWidth() || m_Height != m_RB.GetHeight()) {
        m_RB.Bind();
        m_RB.SetSize(m_Width, m_Height);
        m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
        m_RB.UnBind();
    }
}

void FrameBaker::BeginRenderCatch() {
    auto* w = m_Window;
    m_LastRenderWidth = w->GetRenderFieldWidth();
    m_LastRenderHeight = w->GetRenderFieldHeight();

    w->ChangeViewport(0, 0, m_Width, m_Height);
    m_FB.Bind();

    rc::Renderer::Clear();

    UpdateData();
}

void FrameBaker::EndRenderCatch() {
    m_FB.UnBind();
    m_Window->ChangeViewport(0, 0, m_LastRenderWidth, m_LastRenderHeight);
}

void FrameBaker::SetRenderTexture(const Texture& texture) {
    m_Texture = &texture;
    auto& info = m_TexManager->GetTexInfo(m_Texture);

    m_TexManager->GetTexArray().Bind();

    m_Width = m_Texture->GetWidth();
    m_Height = m_Texture->GetHeight();

    m_FB.Bind();
    m_RB.Bind();

    m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
    rc::AttachTextureArrayToFramebuffer(m_FB, m_TexManager->GetTexArray(), GAPI::ATTACHMENT::COLOR0, info.GetSlot());

    m_RB.UnBind();
    m_FB.UnBind();
}

glm::mat4 FrameBaker::GetProjMatrix() {
    return glm::ortho(0.0f, (float) m_Window->GetWidth(),
                      0.0f, (float) m_Window->GetHeight(),
                      -100.0f, 100.0f);
}

std::shared_ptr<TextureManager> FrameBaker::GetTextureManager() const {
    return m_TexManager;
}
