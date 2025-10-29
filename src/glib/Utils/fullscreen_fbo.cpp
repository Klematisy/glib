#include "fullscreen_fbo.h"

using namespace glib;
namespace rc = RendererCore;

static constexpr float rectangle[] {
    -1.0f, -1.0f,     1.0f, 1.0f, 1.0f, 1.0f,    0, 0, 0,
    -1.0f,  1.0f,     1.0f, 1.0f, 1.0f, 1.0f,    0, 1, 0,
     1.0f,  1.0f,     1.0f, 1.0f, 1.0f, 1.0f,    1, 1, 0,
     1.0f, -1.0f,     1.0f, 1.0f, 1.0f, 1.0f,    1, 0, 0,
};

static constexpr uint32_t inds[] {
    0, 1, 2,
    2, 3, 0
};

FullscreenFBO::FullscreenFBO(GLFWwindow *window) {
    m_VA = std::make_shared<rc::VertexArray>();
    m_VB = std::make_shared<rc::VertexBuffer>(GAPI::DRAW_TYPE::STATIC, sizeof(rectangle), rectangle);
    m_EB = std::make_shared<rc::ElementBuffer>(GAPI::DRAW_TYPE::STATIC, sizeof(inds), inds);

    rc::VertexArrayLayout layout;
    layout.Add<float>(3);
    layout.Add<float>(4);
    layout.Add<float>(3);

    m_VA->AddBuffer(layout, *m_VB);

    m_FB.Bind();
    m_RB.Bind();

    m_TexArr.SetWidth(1024);
    m_TexArr.SetHeight(768);
    m_TexArr.SetLayersCount(1);

    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_S, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);
    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::WRAP_T, GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE);
    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::MAG_FILTER, GAPI::TEXTURE_PARAM::NEAREST);
    m_TexArr.Parameteri(GAPI::TEXTURE_PROPERTY::MIN_FILTER, GAPI::TEXTURE_PARAM::NEAREST);

    m_TexArr.AllocateTexture();

    rc::AttachTextureArrayToFramebuffer(m_FB, m_TexArr, GL_COLOR_ATTACHMENT0, 0);
    rc::AttachFramebufferToRenderbuffer(m_FB, m_RB, GL_DEPTH_STENCIL_ATTACHMENT);

    m_FB.UnBind();
    m_RB.UnBind();
    m_TexArr.Bind(0);

    m_VA->UnBind();
    m_VB->UnBind();
    m_EB->UnBind();
}

void FullscreenFBO::BeginCapture() const {
    glViewport(0, 0, 1024, 786);
    m_FB.Bind();
}

void FullscreenFBO::EndCapture() const {
    m_FB.UnBind();
    glViewport(0, 0, 1024 * 2, 786 * 2);
}

DrawResources FullscreenFBO::GetDrawResources() {
    return {nullptr, m_VA, m_VB, m_EB};
}

