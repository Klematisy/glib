#pragma once


#include "../renderer.h"
#include "../Texture/texture2D.h"

namespace RendererCore {
    class FrameBuffer {
    public:
        FrameBuffer() = default;
        FrameBuffer(uint32_t x, uint32_t y);
        ~FrameBuffer();

        void Bind();
        void UnBind();

        void AddTexture();

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        void SetWidth(uint32_t width);
        void SetHeight(uint32_t height);

    private:
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;

        Texture2D m_texture;
        uint32_t m_FBO = 0;
    };
}