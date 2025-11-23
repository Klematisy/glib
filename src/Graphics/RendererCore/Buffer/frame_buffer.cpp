#include "frame_buffer.h"

using namespace RendererCore;
using namespace GAPI;
static auto& gapi = GraphicsAPIImpl::Get();

Framebuffer::Framebuffer() {
    gapi.CreateFramebuffers(1, &m_FBO);
}

Framebuffer::~Framebuffer() {
    gapi.DeleteFramebuffers(1, &m_FBO);
}

void Framebuffer::Bind() const {
    gapi.BindFramebuffer(m_FBO);
}

void Framebuffer::UnBind() const {
    gapi.BindFramebuffer(0);
}
