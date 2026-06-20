#include "frame_buffer.h"
#include "GraphicsAPI/graphics_api.h"

#define smk_s std::make_shared

template<class T>
using sptr = std::shared_ptr<T>;

VLADLIB_NAMESPACE_USING;

FrameBaker::FrameBaker()
{
    m_Texture->Init(1, 1, 1, {});
    m_RB->Init(image.GetWidth(), image.GetHeight(), GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
    GAPI::AttachFramebufferToRenderbuffer(*m_FB, *m_RB, GAPI::INTERNAL_FORMAT::DEPTH_STENCIL_ATTACHMENT);
    GAPI::AttachTextureArrayToFramebuffer(*m_FB, *m_Texture, GAPI::INTERNAL_FORMAT::COLOR_ATTACHMENT0, 0);
}

void FrameBaker::syncTextureWithImage() {
    if (image.GetTexParams() != m_Texture->GetTexParameters() ||
        image.GetWidth() != m_Texture->GetWidth()             ||
        image.GetHeight() != m_Texture->GetHeight())
    {
        m_RB->Init(image.GetWidth(), image.GetHeight(), GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
        m_Texture->Init(image.GetWidth(), image.GetHeight(), 1, image.GetTexParams());
    }
}

void FrameBaker::StartBake() {
    m_FB->Bind();
}

void FrameBaker::EndBake() {
    m_FB->UnBind();
}
