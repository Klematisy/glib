#pragma once

namespace RendererCore {
    class RenderBuffer {
    public:
        RenderBuffer();
        ~RenderBuffer();

        void Bind();
        void UnBind();

        void RenderbufferStorage(GLenum depthStencil, uint32_t width, uint32_t height);
    private:
        uint32_t m_RB = 0;
    };
}