#pragma once

namespace RendererCore {
    class FrameBuffer;

    class RenderBuffer {
    public:
        RenderBuffer();
        ~RenderBuffer();

        void Bind() const;
        void UnBind() const;

        void RenderbufferStorage(GLenum depthStencil, uint32_t width, uint32_t height);

        friend void AttachFramebufferToRenderbuffer(const FrameBuffer& fb, const RenderBuffer& rb, GLenum depthStencil);
    private:
        uint32_t m_RB = 0;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
    };
}