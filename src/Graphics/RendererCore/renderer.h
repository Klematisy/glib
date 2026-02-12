#pragma once

#include "Graphics/GraphicsAPI/graphics_api_impl.h"

#include "GLFW/glfw3.h"

#include "Buffer/vertex_buffer.h"
#include "Buffer/element_buffer.h"
#include "Buffer/vertex_array.h"
#include "Buffer/frame_buffer.h"
#include "Buffer/render_buffer.h"


#include "Shader/shader_program.h"
#include "Texture/texture2D.h"

#include "Texture/texture_array.h"

#define GlCall(x) RendererCore::GLClearError(); \
                  x;                      \
                  RendererCore::GLLogError();

using namespace GAPI;
static auto& gapi = GraphicsAPIImpl::Get();

namespace RendererCore {
    static void GLClearError() {
        while (glGetError() != GL_NO_ERROR);
    }

    static bool GLLogError() {
        while (GLenum error = glGetError()) {
            std::cout << "[OpenGL Error]: " << error << std::endl;
            return false;
        }
        return true;
    }

    static void GLLogAllErrors() {
        while (1) {
            GLenum error = glGetError();
            if (error == GL_NO_ERROR) break;
            std::cout << "[OpenGL Error]: " << error << std::endl;
        }
    }

    inline void AttachFramebufferToRenderbuffer(const Framebuffer& fb, const Renderbuffer& rb, INTERNAL_FORMAT depthStencil) {
        fb.Bind();
        rb.Bind();

        gapi.FramebufferRenderbuffer(BUFFER_TYPE::FRAME, depthStencil, BUFFER_TYPE::RENDER, rb.m_RB);

        rb.UnBind();
        fb.UnBind();
    }

    inline void AttachTextureToFramebuffer(const Framebuffer& fb, const Texture2D& tex, INTERNAL_FORMAT attachment) {
        fb.Bind();
        tex.Bind(0);

        gapi.FramebufferTexture(BUFFER_TYPE::FRAME, attachment, tex.m_ID, 0);

        tex.UnBind();
        fb.UnBind();
    }

    inline void AttachTextureArrayToFramebuffer(const Framebuffer& fb, const TextureArray& tex, INTERNAL_FORMAT attachment, uint32_t layer) {
        fb.Bind();
        tex.Bind();

        gapi.FramebufferTextureLayer(BUFFER_TYPE::FRAME, attachment, tex.m_ID, 0, layer);

        tex.UnBind();
        fb.UnBind();
    }

    struct GraphicsBuffer {
        std::shared_ptr<RendererCore::VertexArray> vertexArray;
        std::shared_ptr<RendererCore::VertexBuffer> vertexBuffer;
        std::shared_ptr<RendererCore::ElementBuffer> elementBuffer;
    };

    struct RenderStats {
        uint32_t drawCalls = 0;
        uint32_t triangles = 0;
        uint32_t textureBinds = 0;
    };

    class Renderer {
    public:
        Renderer(GAPI::RENDERER_TYPE rendererType = GAPI::RENDERER_TYPE::TRIANGLES, bool DrawEdges = false);

        void Draw(const GraphicsBuffer& gb, const ShaderProgram& shader);
        void Clear();

        void DrawEdges(bool de);
        void SetRendererType(GAPI::RENDERER_TYPE rendererType);
        const RenderStats& GetStats() const;
    private:
        GAPI::RENDERER_TYPE m_RendererType;
        bool m_DrawEdges = false;
        RenderStats m_Stats;
    };
}