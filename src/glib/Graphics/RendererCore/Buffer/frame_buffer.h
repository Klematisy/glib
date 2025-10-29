#pragma once

#include "GL/glew.h"

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

        friend void AttachTextureToFramebuffer(const FrameBuffer& fb, const Texture2D& tex, GLenum attachment);
        friend void AttachTextureArrayToFramebuffer(const FrameBuffer& fb, const TextureArray& tex, GLenum attachment);
        friend void AttachFramebufferToRenderbuffer(const FrameBuffer& fb, const RenderBuffer& rb, GLenum depthStencil);
    private:
        uint32_t m_FBO = 0;
    };
}