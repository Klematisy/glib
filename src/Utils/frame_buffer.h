#pragma once

#include <memory>
#include "Draw/structs.h"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"

namespace glib {
    class FrameBuffer {
    public:
        FrameBuffer(RendererCore::Window* window);

        void BeginCapture();
        void EndCapture();
        void Bind() const;

        DrawResources GetDrawResources();

        int GetWidth() const;
        int GetHeight() const;
        void SetWidth(int width);
        void SetHeight(int height);
    private:
        void UpdateData(uint32_t w, uint32_t h);

        float m_Rectangle[40] {
             0.0f,  0.0f,  1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0, 0, 0,
             0.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    0, 1, 0,
             1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1, 1, 0,
             1.0f,  0.0f,  1.0f,    1.0f, 1.0f, 1.0f, 1.0f,    1, 0, 0
        };

        const uint32_t m_Inds[6] {
            0, 1, 2,
            2, 3, 0
        };

        int m_LastRenderWidth = 0;
        int m_LastRenderHeight = 0;

        int m_Width = 0;
        int m_Height = 0;

        RendererCore::Window* m_Window;

        DrawResources m_DR;

        RendererCore::FrameBuffer m_FB;
        RendererCore::RenderBuffer m_RB;
        RendererCore::TextureArray m_TexArr;
    };
}