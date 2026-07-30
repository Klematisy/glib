#include "frame_buffer.h"
#include "GraphicsAPI/graphics_api.h"

VLADLIB_NAMESPACE_USING;

u0 FrameBaker::Init()
{
    image.Init(1, 1, 1, Range());
    m_FB = GAPI::createFramebuffer();
    m_RB = GAPI::createRenderbuffer();
    m_Texture = GAPI::createTexutreArray(1, 1, 1, {});
    m_RB->Init(image.r_Width, image.r_Height, GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
    GAPI::AttachFramebufferToRenderbuffer(*m_FB, *m_RB, GAPI::INTERNAL_FORMAT::DEPTH_STENCIL_ATTACHMENT);
    GAPI::AttachTextureArrayToFramebuffer(*m_FB, *m_Texture, GAPI::INTERNAL_FORMAT::COLOR_ATTACHMENT0, 0);
}

void FrameBaker::syncTextureWithImage() {
    if (image.texParams != m_Texture->r_TexParameters ||
        image.r_Width != m_Texture->r_Width           ||
        image.r_Height != m_Texture->r_Height)
    {
        m_RB->Init(image.r_Width, image.r_Height, GAPI::INTERNAL_FORMAT::DEPTH24_STENCIL8);
        m_Texture->Init(image.r_Width, image.r_Height, 1, image.texParams);
    }
}

void FrameBaker::StartBake() {
    m_FB->Bind();
}

void FrameBaker::EndBake() {
    m_FB->UnBind();
}
