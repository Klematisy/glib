#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Buffer/VertexBuffer.h"
#include "Buffer/ElementBuffer.h"
#include "Buffer/VertexArray.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture.h"

#include "ShaderCache.h"

namespace GlCore {
    class Renderer {
    public:
        void Draw(const ShaderProgram& shader, const VertexArray& va, const ElementBuffer& eb);
        void Clear();
    };
}