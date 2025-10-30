#pragma once

#include "Graphics/GraphicsAPI/graphics_api_impl.h"

namespace RendererCore {
    class Texture2D;
    class TextureArray;
    class RenderBuffer;

    class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void Bind() const;
        void UnBind() const;

        friend void AttachTextureToFramebuffer(const FrameBuffer& fb, const Texture2D& tex, GAPI::ATTACHMENT attachment);
        friend void AttachTextureArrayToFramebuffer(const FrameBuffer& fb, const TextureArray& tex, GAPI::ATTACHMENT attachment, uint32_t layer);
        friend void AttachFramebufferToRenderbuffer(const FrameBuffer& fb, const RenderBuffer& rb, GAPI::INTERNAL_FORMAT depthStencil);
    private:
        uint32_t m_FBO = 0;
    };
}