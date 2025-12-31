#include "frame_buffer.h"

#define smk_s std::make_shared

template<class T>
using sptr = std::shared_ptr<T>;

using namespace glib;
namespace rc = RendererCore;

static void initTexArrWithParam(sptr<rc::TextureArray>& texArr, GAPI::TEXTURE_PARAM texParam) {
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

FrameBaker::FrameBaker(rc::Window* window)
    : m_Window(window)
{
    sptr<rc::TextureArray> textureArray = smk_s<rc::TextureArray>();
    textureArray->SetWidth(1);
    textureArray->SetHeight(1);
    textureArray->SetLayersCount(1);
    initTexArrWithParam(textureArray, GAPI::TEXTURE_PARAM::NEAREST);
    textureArray->AllocateTexture();

    m_FB.Bind();
    m_RB.Bind();

    m_TexManager = smk_s<TextureManager>();
    m_TexManager->SetTextureArray(textureArray);

    m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
    rc::AttachFramebufferToRenderbuffer(m_FB, m_RB, INTERNAL_FORMAT::DEPTH24_STENCIL8);

    m_FB.UnBind();
    m_RB.UnBind();
}

void FrameBaker::BeginRenderCatch() {
    m_Viewport = {0, 0, m_Window->GetLogicWidth(), m_Window->GetLogicHeight()};
    m_FB.Bind();

    UpdateData();
}

void FrameBaker::EndRenderCatch() {
    m_FB.UnBind();
}

void FrameBaker::UpdateData() {
    if (m_Viewport.width != m_RenderTexture.GetWidth() || m_Viewport.height != m_RenderTexture.GetHeight()) {
        m_TexManager->Clear();

        auto texArr = m_TexManager->GetTexArray();
        if (m_Viewport.width > texArr->GetWidth() || m_Viewport.height > texArr->GetHeight()) {
            texArr->SetWidth(m_Viewport.width);
            texArr->SetHeight(m_Viewport.height);
            texArr->AllocateTexture();
        }

        m_RenderTexture.SetNewTexInfo(m_Viewport.width, m_Viewport.height, 4);

        auto& info = m_TexManager->GetTexInfo(&m_RenderTexture);
        rc::AttachTextureArrayToFramebuffer(m_FB, *texArr, GAPI::ATTACHMENT::COLOR0, info.GetSlot());

        m_TexManager->GetTexInfo(&m_RenderTexture);
    }

    if (m_Viewport.width != m_RB.GetWidth() || m_Viewport.height != m_RB.GetHeight()) {
        m_RB.Bind();
        m_RB.SetSize(m_Viewport.width, m_Viewport.height);
        m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
        m_RB.UnBind();
    }
}

const Texture& FrameBaker::GetRenderTexture() const { return m_RenderTexture; }
std::shared_ptr<TextureManager> FrameBaker::GetTextureManager() const { return m_TexManager; }
