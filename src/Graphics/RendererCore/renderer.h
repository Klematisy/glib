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
        gapi.FramebufferRenderbuffer(BUFFER_TYPE::FRAME, depthStencil, BUFFER_TYPE::RENDER, rb.m_RB);
    }

    inline void AttachTextureToFramebuffer(const Framebuffer& fb, const Texture2D& tex, ATTACHMENT attachment) {
        fb.Bind();
        tex.Bind(0);
        gapi.FramebufferTexture(BUFFER_TYPE::FRAME, attachment, tex.m_TextureId, 0);
    }

    inline void AttachTextureArrayToFramebuffer(const Framebuffer& fb, const TextureArray& tex, ATTACHMENT attachment, uint32_t layer) {
        gapi.FramebufferTextureLayer(BUFFER_TYPE::FRAME, attachment, tex.m_TextureId, 0, layer);
    }

    struct GraphicsBuffer {
        std::shared_ptr<RendererCore::VertexArray> vertexArray;
        std::shared_ptr<RendererCore::VertexBuffer> vertexBuffer;
        std::shared_ptr<RendererCore::ElementBuffer> elementBuffer;
    };

    class Renderer {
    public:
        Renderer(GAPI::RENDERER_TYPE rendererType = GAPI::RENDERER_TYPE::TRIANGLES, bool DrawEdges = false);

        void Draw(const GraphicsBuffer& gb, const ShaderProgram& shader);
        void Clear();

        void DrawEdges(bool de);
        void SetRendererType(GAPI::RENDERER_TYPE rendererType);
    private:
        GAPI::RENDERER_TYPE m_RendererType;
        bool m_DrawEdges = false;
    };
}