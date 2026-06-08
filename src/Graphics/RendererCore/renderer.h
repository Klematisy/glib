#pragma once

#include "Graphics/GraphicsAPI/graphics_api.h"
#include "Graphics/GraphicsAPI/graphics_api_context.h"
#include "Graphics/GraphicsAPI/graphics_api_opengl.h"
#include "Graphics/GraphicsAPI/opengl_glfw_context.h"

#include "Buffer/vertex_buffer.h"
#include "Buffer/element_buffer.h"
#include "Buffer/vertex_array.h"
#include "Buffer/frame_buffer.h"
#include "Buffer/render_buffer.h"

#include "Shader/shader_program.h"
#include "Texture/texture2D.h"

#include "Texture/texture_array.h"
#include <memory>


#define GlCall(x) RendererCore::GLClearError(); \
                  x;                            \
                  RendererCore::GLLogError();

using namespace GAPI;

namespace RendererCore {
    inline IGraphicsAPI* const rendererAPI = &GAPI_OpenGL::Get();
    inline IGraphicsAPIContext* const rendererContext = &OpenGLGLFWContext::Get();

    struct RendererContext {
        uint32_t majVer = 4;
        uint32_t minVer = 1;
    };

    static void GLClearError() {
        // while (glGetError() != GL_NO_ERROR);
    }

    static bool GLLogError() {
        // while (GLenum error = glGetError()) {
        //     LOGWARN("Graphics Error: " + std::to_string(error));
        //     return false;
        // }
        return true;
    }

    static void GLLogAllErrors() {
        // while (1) {
        //     GLenum error = glGetError();
        //     if (error == GL_NO_ERROR) break;
        //     LOGWARN("Graphics Error: " + std::to_string(error));
        // }
    }

    inline void AttachFramebufferToRenderbuffer(const Framebuffer& fb, const Renderbuffer& rb, INTERNAL_FORMAT depthStencil) {
        fb.Bind();
        rb.Bind();

        rendererAPI->FramebufferRenderbuffer(BUFFER_TYPE::FRAME, depthStencil, BUFFER_TYPE::RENDER, rb.m_RB);

        rb.UnBind();
        fb.UnBind();
    }

    inline void AttachTextureToFramebuffer(const Framebuffer& fb, const Texture2D& tex, INTERNAL_FORMAT attachment) {
        fb.Bind();
        tex.Bind(0);

        rendererAPI->FramebufferTexture(BUFFER_TYPE::FRAME, attachment, tex.m_ID, 0);

        tex.UnBind();
        fb.UnBind();
    }

    inline void AttachTextureArrayToFramebuffer(const Framebuffer& fb, const TextureArray& tex, INTERNAL_FORMAT attachment, uint32_t layer) {
        fb.Bind();
        tex.Bind();

        rendererAPI->FramebufferTextureLayer(BUFFER_TYPE::FRAME, attachment, tex.m_ID, 0, layer);

        tex.UnBind();
        fb.UnBind();
    }

    struct RenderItem {
        std::unique_ptr<RendererCore::VertexArray> vertexArray;
        std::unique_ptr<RendererCore::VertexBuffer> vertexBuffer;
        std::unique_ptr<RendererCore::ElementBuffer> elementBuffer;

        const ShaderProgram* shader = nullptr;
        const ITexture* texture = nullptr;
        GAPI::RENDERER_TYPE renderType = GAPI::RENDERER_TYPE::TRIANGLES;
    };

    struct RenderStats {
        uint32_t drawCalls = 0;
        uint32_t triangles = 0;
        uint32_t textureBinds = 0;
    };

    class Renderer {
    public:
        void Draw(const RenderItem& gb);
        void Clear();

        void DrawEdges(bool de);
        RenderStats GetStats() const;
    private:
        bool m_DrawEdges = false;
        mutable RenderStats m_Stats;
    };
}
