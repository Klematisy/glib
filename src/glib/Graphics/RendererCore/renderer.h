#pragma once

#include "Graphics/GraphicsAPI/graphics_api_impl.h"

#include "GLFW/glfw3.h"

#include "Buffer/vertex_buffer.h"
#include "Buffer/element_buffer.h"
#include "Buffer/vertex_array.h"
#include "Buffer/frame_buffer.h"
#include "Buffer/render_buffer.h"


#include "Shader/shader_program.h"
#include "Graphics/RendererCore/Texture/texture2D.h"

#include "Texture/texture_array.h"

#define GlCall(x) GlCore::GLClearError(); \
                  x;                      \
                  GlCore::GLLogError();

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

    inline void AttachFramebufferToRenderbuffer(const FrameBuffer& fb, const RenderBuffer& rb, GLenum depthStencil) {
        fb.Bind();
        rb.Bind();

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, depthStencil, GL_RENDERBUFFER, rb.m_RB);
    }

    inline void AttachTextureToFramebuffer(const FrameBuffer& fb, const Texture2D& tex, GLenum attachment) {
        fb.Bind();
        tex.Bind(0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex.m_TextureId, 0);
    }

    inline void AttachTextureArrayToFramebuffer(const FrameBuffer& fb, const TextureArray& tex, GLenum attachment, uint32_t layer) {
        fb.Bind();
        tex.Bind(0);
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex.m_TextureId, 0, 0);
    }

    class Renderer {
    public:
        void Draw(const ShaderProgram& shader, const VertexArray& va, const ElementBuffer& eb);
        void Clear();
    };
}