#include "fullscreen_fbo.h"

using namespace glib;
namespace rc = RendererCore;

static constexpr float rectangle[] {
    -1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0, 0, 0,
    -1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0, 1, 0,
     1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1, 1, 0,
     1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1, 0, 0
};

static constexpr uint32_t inds[] {
    0, 1, 2,
    2, 3, 0
};

FullscreenFBO::FullscreenFBO(GLFWwindow* window) {
    m_Window = window;
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);

    m_VA = std::make_shared<rc::VertexArray>();
    m_VB = std::make_shared<rc::VertexBuffer>(GAPI::DRAW_TYPE::STATIC, sizeof(rectangle), rectangle);
    m_EB = std::make_shared<rc::ElementBuffer>(GAPI::DRAW_TYPE::STATIC, 6, inds);

    m_VA->Bind();
    m_VB->Bind();
    m_EB->Bind();

    rc::VertexArrayLayout layout;
    layout.Add<float>(3);
    layout.Add<float>(4);
    layout.Add<float>(3);

    m_VA->AddBuffer(layout, *m_VB);

    m_FB.Bind();
    m_RB.Bind();

    m_TexArr.SetWidth(windowWidth);
    m_TexArr.SetHeight(windowHeight);
    m_TexArr.SetLayersCount(2);

    m_TexArr.AllocateTexture();
    m_TexArr.Bind(0);

    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_S, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);
    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_T, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);
    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::MAG_FILTER, GAPI::TEXTURE_PARAM::NEAREST);
    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::MIN_FILTER, GAPI::TEXTURE_PARAM::NEAREST);

    rc::AttachTextureArrayToFramebuffer(m_FB, m_TexArr, GL_COLOR_ATTACHMENT0, 0);

    m_RB.SetSize(windowWidth, windowHeight);
    m_RB.RenderbufferStorage(GL_DEPTH24_STENCIL8);
    rc::AttachFramebufferToRenderbuffer(m_FB, m_RB, GL_DEPTH_STENCIL_ATTACHMENT);

    m_FB.UnBind();
    m_RB.UnBind();
    m_TexArr.UnBind();

    m_VA->UnBind();
    m_VB->UnBind();
    m_EB->UnBind();
}

void FullscreenFBO::UpdateData(uint32_t w, uint32_t h) {
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
        m_RB.RenderbufferStorage(GL_DEPTH24_STENCIL8);
        m_RB.UnBind();
    }
}

void FullscreenFBO::BeginCapture() {
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);

    glViewport(0, 0, windowWidth, windowHeight);
    m_FB.Bind();

    UpdateData(windowWidth, windowHeight);

    glEnable(GL_DEPTH_TEST);
}

void FullscreenFBO::EndCapture() {
    m_FB.UnBind();
    glDisable(GL_DEPTH_TEST);
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(m_Window, &fbWidth, &fbHeight);

    glViewport(0, 0, fbWidth, fbHeight);
}

DrawResources FullscreenFBO::GetDrawResources() {
    return {nullptr, m_VA, m_VB, m_EB};
}

void FullscreenFBO::Bind() const {
    m_TexArr.Bind(0);
}
