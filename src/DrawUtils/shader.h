#pragma once

#include <unordered_set>
#include <functional>
#include <vector>
#include <memory>
#include <cstring>
#include <utility>

#include "Graphics/RendererCore/Shader/shader_program.h"
#include "environment.h"

VLADLIB_NAMESPACE_OPEN

class Shader {
    using glcore_sp = RendererCore::ShaderProgram;
public:
    Shader() = default;

    template<class... Args>
    void AddSrcFiles(Args&... args);
    void Compile();
    void HotReload();
    glcore_sp* GetShaderProgram() const;
private:
    uint32_t m_AddedCount = 0;

    std::vector<const char*> m_FilePaths;
    std::shared_ptr<glcore_sp> m_Program = std::make_shared<glcore_sp>();
};

template<class... Args>
void Shader::AddSrcFiles(Args&... args) {
    std::vector<const char*> filePaths({std::forward<Args>(args)...});
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

VLADLIB_NAMESPACE_CLOSE