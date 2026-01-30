#include "render_buffer.h"

using namespace RendererCore;
using namespace GAPI;
static auto& gapi = GraphicsAPIImpl::Get();

Renderbuffer::Renderbuffer() {
    gapi.CreateRenderbuffers(1, &m_RB);
}

Renderbuffer::~Renderbuffer() {
    gapi.DeleteRenderbuffers(1, &m_RB);
}

void Renderbuffer::Bind() const {
    gapi.BindRenderbuffer(m_RB);
}

void Renderbuffer::UnBind() const {
    gapi.BindRenderbuffer(0);
}

void Renderbuffer::SetSize(uint32_t width, uint32_t height) {
    m_Width = width;
    m_Height = height;
}

uint32_t Renderbuffer::GetWidth() const {
    return m_Width;
}

uint32_t Renderbuffer::GetHeight() const {
    return m_Height;
}

void Renderbuffer::RenderbufferStorage(INTERNAL_FORMAT depthStencil) {
    Bind();
    gapi.RenderbufferStorage(depthStencil, m_Width, m_Height);
    UnBind();
}
