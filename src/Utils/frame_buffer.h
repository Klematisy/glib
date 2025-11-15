#pragma once

#include <memory>
#include <array>
#include "Draw/structs.h"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"

namespace glib {
    class Framebuffer {
    public:
        Framebuffer(RendererCore::Window* window);

        void BeginCapture();
        void EndCapture();
        void Bind() const;

        DrawResources GetDrawResources();

        int GetWidth() const;
        int GetHeight() const;
        void SetWidth(int width);
        void SetHeight(int height);

        const std::array<Vertex, 4>& GetVertices() const;
        const std::array<uint32_t, 6>& GetIndices() const;
    private:
        void UpdateData(uint32_t w, uint32_t h);

        std::array<Vertex, 4> m_Rectangle;

        const std::array<uint32_t, 6> m_Indices {
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