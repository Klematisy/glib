#include "frame_buffer.h"
#include "../../GraphicsAPI/graphics_api_impl.h"

using namespace RendererCore;
using namespace GAPI;
static auto &gapi = GraphicsAPIImpl::Get();

FrameBuffer::FrameBuffer() {
    gapi.CreateFramebuffers(1, &m_FBO);
}

FrameBuffer::~FrameBuffer() {
    gapi.DeleteFramebuffers(1, &m_FBO);
}

void FrameBuffer::Bind() const {
    gapi.BindFramebuffer(m_FBO);
}

void FrameBuffer::UnBind() const {
    gapi.BindFramebuffer(0);
}
