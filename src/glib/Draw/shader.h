#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <cstring>

#include "OpenGLCore/Shader/shader_program.h"

namespace glib {

    class Shader {
        using glcore_sp = GlCore::ShaderProgram;
    public:
        Shader() = default;

        void AddSrcFiles(const std::vector<const char*>& filePaths);
        void Compile();
        void HotReload();
        std::shared_ptr<glcore_sp> GetShaderProgram();
    private:
        uint32_t m_AddedCount = 0;

        std::vector<const char*> m_FilePaths;
        std::shared_ptr<glcore_sp> m_Program = std::make_shared<glcore_sp>();
    };
}