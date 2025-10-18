#pragma once

#include <vector>
#include <memory>
#include <unordered_set>
#include <cstring>
#include <utility>

#include "OpenGLCore/Shader/shader_program.h"

namespace glib {

    class Shader {
        using glcore_sp = GlCore::ShaderProgram;
    public:
        Shader() = default;

        template<class... Args>
        Shader(Args&... args);

        template<class... Args>
        void AddSrcFiles(Args&... args);
        void Compile();
        void HotReload();
        std::shared_ptr<glcore_sp> GetShaderProgram();
    private:
        uint32_t m_AddedCount = 0;

        std::vector<const char *> m_FilePaths;
        std::shared_ptr<glcore_sp> m_Program = std::make_shared<glcore_sp>();
    };

    template<class... Args>
    void Shader::AddSrcFiles(Args&... args) {
        std::vector<const char *> filePaths({std::forward<Args>(args)...});
        m_AddedCount = 0;

        for (auto inputPath: filePaths) {
            bool filePathAlreadyExists = false;
            for (auto existsPath: m_FilePaths) {
                if (strcmp(existsPath, inputPath) == 0) {
                    filePathAlreadyExists = true;
                }
            }
            if (!filePathAlreadyExists) {
                m_FilePaths.push_back(inputPath);
                m_AddedCount++;
            }
        }
    }

    template<class... Args>
    Shader::Shader(Args&... args) {
        AddSrcFiles(std::forward<Args>(args)...);
    }
}