#include "frame_buffer.h"

#define smk_s std::make_shared

template<class T>
using sptr = std::shared_ptr<T>;

using namespace glib;
namespace rc = RendererCore;

FrameBaker::FrameBaker(rc::Window* window)
    : m_Window(window)
{
    m_FB.Bind();
    m_RB.Bind();

    m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
    rc::AttachFramebufferToRenderbuffer(m_FB, m_RB, INTERNAL_FORMAT::DEPTH24_STENCIL8);

    m_FB.UnBind();
    m_RB.UnBind();
}

void FrameBaker::StartBake() {
    m_FB.Bind();
    m_Window->ChangeViewport(m_Window->GetViewport());
}

void FrameBaker::EndBake() {
    m_FB.UnBind();
}

const RendererCore::Framebuffer &FrameBaker::GetFrameBuffer() const {
    return m_FB;
}
