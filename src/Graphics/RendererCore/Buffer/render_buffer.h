#pragma once

#include "../../GraphicsAPI/graphics_api_impl.h"

namespace RendererCore {
    class FrameBuffer;

    class RenderBuffer {
    public:
        RenderBuffer();
        ~RenderBuffer();

        void SetSize(uint32_t width, uint32_t height);

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        void Bind() const;
        void UnBind() const;

        void RenderbufferStorage(GAPI::INTERNAL_FORMAT depthStencil);

        friend void AttachFramebufferToRenderbuffer(const FrameBuffer& fb, const RenderBuffer& rb, GAPI::INTERNAL_FORMAT depthStencil);
    private:
        uint32_t m_RB = 0;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
    };
}