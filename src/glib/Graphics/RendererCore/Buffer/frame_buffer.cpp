#include "frame_buffer.h"

using namespace RendererCore;

FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &m_FBO);
}

FrameBuffer::~FrameBuffer() {
    glDeleteBuffers(1, &m_FBO);
}

void FrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void FrameBuffer::UnBind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
