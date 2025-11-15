#include "frame_buffer.h"

using namespace glib;
namespace rc = RendererCore;

FrameBuffer::FrameBuffer(RendererCore::Window* window) {
    m_Window = window;

    m_DR.vertexArray = std::make_shared<rc::VertexArray>();
    m_DR.vertexBuffer = std::make_shared<rc::VertexBuffer>(GAPI::DRAW_TYPE::DYNAMIC, sizeof(float) * 10 * 4, m_Rectangle);
    m_DR.elementBuffer = std::make_shared<rc::ElementBuffer>(GAPI::DRAW_TYPE::DYNAMIC, 6, m_Inds);

    m_DR.vertexArray->Bind();
    m_DR.vertexBuffer->Bind();
    m_DR.elementBuffer->Bind();

    rc::VertexArrayLayout layout;
    layout.Add<float>(3);
    layout.Add<float>(4);
    layout.Add<float>(3);

    m_DR.vertexArray->AddBuffer(layout, *m_DR.vertexBuffer);

    m_DR.vertexArray->UnBind();
    m_DR.vertexBuffer->UnBind();
    m_DR.elementBuffer->UnBind();

    m_DR.vertexArray->Bind();
    m_FB.Bind();
    m_RB.Bind();

    m_TexArr.SetLayersCount(1);

    m_TexArr.AllocateTexture();
    m_TexArr.Bind(0);

    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_S, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);
    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_T, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);
    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::MAG_FILTER, GAPI::TEXTURE_PARAM::NEAREST);
    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::MIN_FILTER, GAPI::TEXTURE_PARAM::NEAREST);

    rc::AttachTextureArrayToFramebuffer(m_FB, m_TexArr, GAPI::ATTACHMENT::COLOR0, 0);

    m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
    rc::AttachFramebufferToRenderbuffer(m_FB, m_RB, INTERNAL_FORMAT::DEPTH24_STENCIL8);

    m_FB.UnBind();
    m_RB.UnBind();
    m_TexArr.UnBind();
}

void FrameBuffer::UpdateData(uint32_t w, uint32_t h) {
    if (w != m_TexArr.GetWidth() || h != m_TexArr.GetHeight()) {
        m_TexArr.SetWidth(w);
        m_TexArr.SetHeight(h);
        m_TexArr.Bind(0);

        m_TexArr.AllocateTexture();

        m_TexArr.UnBind();
    }

    if (w != m_RB.GetWidth() || h != m_RB.GetHeight()) {
        m_RB.Bind();
        m_RB.SetSize(w, h);
        m_RB.RenderbufferStorage(GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
        m_RB.UnBind();
    }
}

void FrameBuffer::BeginCapture() {
    m_LastRenderWidth = m_Window->GetRenderFieldWidth();
    m_LastRenderHeight = m_Window->GetRenderFieldHeight();

    m_Window->ChangeViewport(m_Width, m_Height);
    m_FB.Bind();

    UpdateData(m_Width, m_Height);
}

void FrameBuffer::EndCapture() {
    m_FB.UnBind();

    m_Window->ChangeViewport(m_LastRenderWidth, m_LastRenderHeight);
}

DrawResources FrameBuffer::GetDrawResources() {
    return m_DR;
}

void FrameBuffer::Bind() const {
    m_TexArr.Bind(0);
}

int FrameBuffer::GetWidth() const {
    return m_Width;
}

void FrameBuffer::SetWidth(int width) {
    if (m_Width == width) return;
    m_Width = width;
    m_Rectangle[20] = (float) width;
    m_Rectangle[30] = (float) width;

    m_DR.vertexBuffer->PutData(sizeof(float) * 40, m_Rectangle);
}

int FrameBuffer::GetHeight() const {
    return m_Height;
}

void FrameBuffer::SetHeight(int height) {
    if (m_Height == height) return;

    m_Height = height;
    m_Rectangle[11] = (float) height;
    m_Rectangle[21] = (float) height;

    m_DR.vertexBuffer->PutData(sizeof(float) * 40, m_Rectangle);
}
