#pragma once

#include "Graphics/GraphicsAPI/graphics_api_impl.h"

#include "GLFW/glfw3.h"

#include "Buffer/vertex_buffer.h"
#include "Buffer/element_buffer.h"
#include "Buffer/vertex_array.h"
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

    class Renderer {
    public:
        void Draw(const ShaderProgram& shader, const VertexArray& va, const ElementBuffer& eb);
        void Clear();
    };
}