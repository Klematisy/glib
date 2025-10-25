#include "frame_buffer.h"

using namespace RendererCore;

FrameBuffer::FrameBuffer(uint32_t x, uint32_t y)
    : m_Width(x), m_Height(y)
{
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void FrameBuffer::AddTexture() {

}
