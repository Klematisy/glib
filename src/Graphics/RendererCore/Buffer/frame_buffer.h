#pragma once

#include "Graphics/GraphicsAPI/graphics_api_impl.h"

namespace RendererCore {
    class Texture2D;
    class TextureArray;
    class Renderbuffer;

    class Framebuffer {
    public:
        Framebuffer();
        ~Framebuffer();

        void Bind() const;
        void UnBind() const;

        friend void AttachTextureToFramebuffer(const Framebuffer& fb, const Texture2D& tex, GAPI::INTERNAL_FORMAT attachment);
        friend void AttachTextureArrayToFramebuffer(const Framebuffer& fb, const TextureArray& tex, GAPI::INTERNAL_FORMAT attachment, uint32_t layer);
        friend void AttachFramebufferToRenderbuffer(const Framebuffer& fb, const Renderbuffer& rb, GAPI::INTERNAL_FORMAT depthStencil);
    private:
        uint32_t m_FBO = 0;
    };
}