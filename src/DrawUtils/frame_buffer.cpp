#include "frame_buffer.h"

#define smk_s std::make_shared

template<class T>
using sptr = std::shared_ptr<T>;

using namespace glib;
namespace rc = RendererCore;

FrameBaker::FrameBaker()
{
    m_RB.SetSize(3000, 3000);
    m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
    rc::AttachFramebufferToRenderbuffer(m_FB, m_RB, INTERNAL_FORMAT::DEPTH_STENCIL_ATTACHMENT);
}

void FrameBaker::StartBake() {
    m_FB.Bind();
}

void FrameBaker::EndBake() {
    m_FB.UnBind();
}

const RendererCore::Framebuffer &FrameBaker::GetFrameBuffer() const {
    return m_FB;
}
