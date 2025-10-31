#include "GL/glew.h"

#include "render_buffer.h"

using namespace RendererCore;
using namespace GAPI;
static auto& gapi = GraphicsAPIImpl::Get();

RenderBuffer::RenderBuffer() {
    gapi.CreateRenderbuffers(1, &m_RB);
}

RenderBuffer::~RenderBuffer() {
    gapi.DeleteRenderbuffers(1, &m_RB);
}

void RenderBuffer::Bind() const {
    gapi.BindRenderbuffer(m_RB);
}

void RenderBuffer::UnBind() const {
    gapi.BindRenderbuffer(0);
}

void RenderBuffer::SetSize(uint32_t width, uint32_t height) {
    m_Width = width;
    m_Height = height;
}

uint32_t RenderBuffer::GetWidth() const {
    return m_Width;
}

uint32_t RenderBuffer::GetHeight() const {
    return m_Height;
}

void RenderBuffer::RenderbufferStorage(INTERNAL_FORMAT depthStencil) {
    gapi.RenderbufferStorage(depthStencil, m_Width, m_Height);
}
