#pragma once

#include <memory>
#include "Draw/structs.h"

#include "Graphics/RendererCore/renderer.h"

namespace glib {
    class FullscreenFBO {
    public:
        FullscreenFBO(GLFWwindow* window);

        void BeginCapture();
        void EndCapture();

        void Bind() const;

        DrawResources GetDrawResources();
    private:
        void UpdateData(uint32_t w, uint32_t h);

        GLFWwindow* m_Window;

        std::shared_ptr<RendererCore::VertexArray> m_VA;
        std::shared_ptr<RendererCore::VertexBuffer> m_VB;
        std::shared_ptr<RendererCore::ElementBuffer> m_EB;

        RendererCore::FrameBuffer m_FB;
        RendererCore::RenderBuffer m_RB;
        RendererCore::TextureArray m_TexArr;
    };
}