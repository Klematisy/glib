#pragma once


#include "../renderer.h"
#include "../Texture/texture2D.h"

namespace RendererCore {
    class FrameBuffer {
    public:
        FrameBuffer();
        ~FrameBuffer();

        void Bind();
        void UnBind();
    private:
        uint32_t m_FBO = 0;
    };
}