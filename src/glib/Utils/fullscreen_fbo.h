#pragma once

#include <memory>
#include "Draw/structs.h"

#include "Graphics/RendererCore/renderer.h"

namespace glib {
    class FullscreenFBO {
    public:
        FullscreenFBO(GLFWwindow* window);

        void BeginCapture() const;
        void EndCapture() const;

        DrawResources GetDrawResources();
    private:
        std::shared_ptr<RendererCore::VertexArray> m_VA;
        std::shared_ptr<RendererCore::VertexBuffer> m_VB;
        std::shared_ptr<RendererCore::ElementBuffer> m_EB;

        RendererCore::FrameBuffer m_FB;
        RendererCore::RenderBuffer m_RB;
        RendererCore::TextureArray m_TexArr;

        GLFWwindow* m_Window;
    };
}