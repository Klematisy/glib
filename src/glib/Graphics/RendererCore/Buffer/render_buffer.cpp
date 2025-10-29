#include "GL/glew.h"

#include "render_buffer.h"

using namespace RendererCore;

RenderBuffer::RenderBuffer() {
    glGenRenderbuffers(1, &m_RB);
}

RenderBuffer::~RenderBuffer() {
    glDeleteRenderbuffers(1, &m_RB);
}

void RenderBuffer::Bind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, m_RB);
}

void RenderBuffer::UnBind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
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

void RenderBuffer::RenderbufferStorage(GLenum depthStencil) {
    glRenderbufferStorage(GL_RENDERBUFFER, depthStencil, (int) m_Width, (int) m_Height);
}
