#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Buffer/VertexBuffer.h"
#include "Buffer/ElementBuffer.h"
#include "Buffer/VertexArray.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture.h"

namespace GlCore {
    class Renderer {
    public:
        void Draw(const ShaderProgram& shader, const VertexArray& va, const ElementBuffer& eb);
        void Clear();
    };

    class ShaderCache {
    public:
        static ShaderCache& GetCache();

        void LoadCache();
        const std::string& GetTemplate();
        ShaderProgram &GetBasicProgram();
    private:
        ShaderProgram m_BasicShader;
        std::string m_ShaderTemplate;

        ShaderCache() = default;
        static ShaderCache s_CacheInstance;
    };

    inline ShaderCache ShaderCache::s_CacheInstance;
}