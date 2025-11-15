#include "frame_buffer.h"

using namespace glib;
namespace rc = RendererCore;

Framebuffer::Framebuffer(RendererCore::Window* window) {
    m_Window = window;

    m_Rectangle[0] = { {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0, 0} };
    m_Rectangle[1] = { {0.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 1, 0} };
    m_Rectangle[2] = { {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1, 1, 0} };
    m_Rectangle[3] = { {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1, 0, 0} };

    m_DR.vertexArray = std::make_shared<rc::VertexArray>();
    m_DR.vertexBuffer = std::make_shared<rc::VertexBuffer>(GAPI::DRAW_TYPE::DYNAMIC, sizeof(Vertex) * 4, m_Rectangle.data());
    m_DR.elementBuffer = std::make_shared<rc::ElementBuffer>(GAPI::DRAW_TYPE::DYNAMIC, m_Indices.size(), m_Indices.data());

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

void Framebuffer::UpdateData(uint32_t w, uint32_t h) {
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

void Framebuffer::BeginCapture() {
    m_LastRenderWidth = m_Window->GetRenderFieldWidth();
    m_LastRenderHeight = m_Window->GetRenderFieldHeight();

    m_Window->ChangeViewport(m_Width, m_Height);
    m_FB.Bind();

    UpdateData(m_Width, m_Height);
}

void Framebuffer::EndCapture() {
    m_FB.UnBind();

    m_Window->ChangeViewport(m_LastRenderWidth, m_LastRenderHeight);
}

DrawResources Framebuffer::GetDrawResources() {
    return m_DR;
}

void Framebuffer::Bind() const {
    m_TexArr.Bind(0);
}

int Framebuffer::GetWidth() const {
    return m_Width;
}

void Framebuffer::SetWidth(int width) {
    if (m_Width == width) return;
    m_Width = width;
    m_Rectangle[2].position.x = (float) width;
    m_Rectangle[3].position.x = (float) width;

    m_DR.vertexBuffer->PutData(sizeof(Vertex) * 4, m_Rectangle.data());
}

int Framebuffer::GetHeight() const {
    return m_Height;
}

void Framebuffer::SetHeight(int height) {
    if (m_Height == height) return;

    m_Height = height;
    m_Rectangle[1].position.y = (float) height;
    m_Rectangle[2].position.y = (float) height;

    m_DR.vertexBuffer->PutData(sizeof(Vertex) * 4, m_Rectangle.data());
}

const std::array<Vertex, 4>& Framebuffer::GetVertices() const {
    return m_Rectangle;
}

const std::array<uint32_t, 6>& Framebuffer::GetIndices() const {
    return m_Indices;
}
