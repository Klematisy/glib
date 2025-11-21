#include "frame_buffer.h"

using namespace glib;
namespace rc = RendererCore;


Framebuffer::Framebuffer(RendererCore::Window *window) {
    m_Window = window;
    m_ContentsBuffer = CreateDrawBasicsResources();
}

DrawBuffer& Framebuffer::GetBuffer() { return m_ContentsBuffer; }

FrameBaker::FrameBaker(RendererCore::Window* window)
        : Framebuffer(window)
{
    m_ContentsBuffer.vertexArray->Bind();

    m_FB.Bind();
    m_RB.Bind();

    m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
    rc::AttachFramebufferToRenderbuffer(m_FB, m_RB, INTERNAL_FORMAT::DEPTH24_STENCIL8);

    m_FB.UnBind();
    m_RB.UnBind();
}

void FrameBaker::UpdateData() {
    if (m_Width != m_RB.GetWidth() || m_Height != m_RB.GetHeight()) {
        m_RB.Bind();
        m_RB.SetSize(m_Width, m_Height);
        m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
        m_RB.UnBind();
    }
}

void FrameBaker::BeginRenderCatch() {
    m_LastRenderWidth = m_Window->GetRenderFieldWidth();
    m_LastRenderHeight = m_Window->GetRenderFieldHeight();

    m_Window->ChangeViewport(0, 0, m_Width, m_Height);
    m_FB.Bind();

    UpdateData();
}

void FrameBaker::EndRenderCatch() {
    m_FB.UnBind();

    m_Window->ChangeViewport(0, 0, m_LastRenderWidth, m_LastRenderHeight);
}

void FrameBaker::SetRenderTexture(TextureManager& tm, const Texture& tex) {
    auto& info = tm.GetTexInfo(&tex);

    tm.GetTexArray().Bind();

    m_Width = tex.GetWidth();
    m_Height = tex.GetHeight();

    m_FB.Bind();
    m_RB.Bind();

    m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
    rc::AttachTextureArrayToFramebuffer(m_FB, tm.GetTexArray(), GAPI::ATTACHMENT::COLOR0, info.GetSlot());

    m_RB.UnBind();
    m_FB.UnBind();
}