#include "render_buffer.h"
#include "../renderer.h"

using namespace RendererCore;
using namespace GAPI;

static const auto gapi = rendererAPI;

Renderbuffer::Renderbuffer() {
    gapi->CreateRenderbuffers(1, &m_RB);
}

Renderbuffer::~Renderbuffer() {
    gapi->DeleteRenderbuffers(1, &m_RB);
}

void Renderbuffer::Bind() const {
    gapi->BindRenderbuffer(m_RB);
}

void Renderbuffer::UnBind() const {
    gapi->BindRenderbuffer(0);
}

uint32_t Renderbuffer::GetWidth() const {
    return m_Width;
}

uint32_t Renderbuffer::GetHeight() const {
    return m_Height;
}

void Renderbuffer::Init(uint32_t width, uint32_t height, GAPI::INTERNAL_FORMAT depthStencil) {
    Bind();
    m_Width = width;
    m_Height = height;
    gapi->RenderbufferStorage(depthStencil, m_Width, m_Height);
    UnBind();
}
