#include "frame_buffer.h"

#define smk_s std::make_shared

template<class T>
using sptr = std::shared_ptr<T>;

VLADLIB_NAMESPACE_USING;
namespace rc = RendererCore;

FrameBaker::FrameBaker()
{
    m_Texture.Init(1, 1, 1, {});
    m_RB.Init(image.GetWidth(), image.GetHeight(), INTERNAL_FORMAT::DEPTH24_STENCIL8);
    rc::AttachFramebufferToRenderbuffer(m_FB, m_RB, INTERNAL_FORMAT::DEPTH_STENCIL_ATTACHMENT);
    rc::AttachTextureArrayToFramebuffer(m_FB, m_Texture, INTERNAL_FORMAT::COLOR_ATTACHMENT0, 0);
}

void FrameBaker::syncTextureWithImage() {
    if (image.GetTexParams() != m_Texture.GetTexParameters() ||
        image.GetWidth() != m_Texture.GetWidth()             ||
        image.GetHeight() != m_Texture.GetHeight())
    {
        m_RB.Init(image.GetWidth(), image.GetHeight(), INTERNAL_FORMAT::DEPTH24_STENCIL8);
        m_Texture.Init(image.GetWidth(), image.GetHeight(), 1, image.GetTexParams());
    }
}

void FrameBaker::StartBake() {
    m_FB.Bind();
}

void FrameBaker::EndBake() {
    m_FB.UnBind();
}
