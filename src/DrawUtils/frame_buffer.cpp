#include "frame_buffer.h"

#define smk_s std::make_shared

template<class T>
using sptr = std::shared_ptr<T>;

using namespace glib;
namespace rc = RendererCore;

FrameBaker::FrameBaker()
{
    m_Texture.Init(0, 0, 1, {});
    m_RB.Init(m_Image.GetWidth(), m_Image.GetHeight(), INTERNAL_FORMAT::DEPTH24_STENCIL8);
    rc::AttachFramebufferToRenderbuffer(m_FB, m_RB, INTERNAL_FORMAT::DEPTH_STENCIL_ATTACHMENT);
    rc::AttachTextureArrayToFramebuffer(m_FB, m_Texture, INTERNAL_FORMAT::COLOR_ATTACHMENT0, 0);
}

void FrameBaker::syncTextureWithImage() {
    if (m_Image.GetTexParams() != m_Texture.GetTexParameters() ||
        m_Image.GetWidth() != m_Texture.GetWidth()             ||
        m_Image.GetHeight() != m_Texture.GetHeight())
    {
        m_RB.Init(m_Image.GetWidth(), m_Image.GetHeight(), INTERNAL_FORMAT::DEPTH24_STENCIL8);
        m_Texture.Init(m_Image.GetWidth(), m_Image.GetHeight(), 1, m_Image.GetTexParams());
    }
}

void FrameBaker::StartBake() {
    m_FB.Bind();
}

void FrameBaker::EndBake() {
    m_FB.UnBind();
}

rc::ImageInfo& FrameBaker::GetImage() {
    return m_Image;
}
