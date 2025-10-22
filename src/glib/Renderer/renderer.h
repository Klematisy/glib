#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "GraphicsAPI/Buffer/vertex_buffer.h"
#include "GraphicsAPI/Buffer/element_buffer.h"
#include "GraphicsAPI/Buffer/vertex_array.h"
#include "OpenGL/Shader/shader_program.h"
#include "OpenGL/Texture/texture.h"

#include "OpenGL/Texture/texture_array.h"

#define GlCall(x) GlCore::GLClearError(); \
                  x;                      \
                  GlCore::GLLogError();

namespace GlCore {
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