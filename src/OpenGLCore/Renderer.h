#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Buffer/VertexBuffer.h"
#include "Buffer/ElementBuffer.h"
#include "Buffer/VertexArray.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture.h"

#include "ShaderCache.h"

#include "Texture/TextureArray.h"

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