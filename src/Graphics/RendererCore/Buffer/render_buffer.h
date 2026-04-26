#pragma once

#include "Graphics/GraphicsAPI/graphics_api_impl.h"

namespace RendererCore {
    class Framebuffer;

    class Renderbuffer {
    public:
        Renderbuffer();
        ~Renderbuffer();

        void Init(uint32_t width, uint32_t height, GAPI::INTERNAL_FORMAT depthStencil);

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        void Bind() const;
        void UnBind() const;

        friend void AttachFramebufferToRenderbuffer(const Framebuffer& fb, const Renderbuffer& rb, GAPI::INTERNAL_FORMAT depthStencil);
    private:
        uint32_t m_RB = 0;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
    };
}