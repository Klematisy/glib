#pragma once

#include <vector>
#include <memory>
#include <cstring>

#include "GraphicsAPI/graphics_api.h"
#include "environment.h"

VLADLIB_NAMESPACE_OPEN

class Shader {
    using glcore_sp = GAPI::ShaderProgram;
public:
    Shader();

    void AddSrcFile(const char* filePath, GAPI::SHADER_TYPE types);
    void Compile();
    void HotReload();
    bool IsValid() const;
    glcore_sp* GetShaderProgram() const;
private:
    uint32_t m_AddedCount = 0;
    std::shared_ptr<GAPI::ShaderCompiler> m_Compiler = nullptr;

    std::vector<std::pair<const char*, GAPI::SHADER_TYPE>> m_Shaders;
    std::shared_ptr<glcore_sp> m_Program = GAPI::createShaderProgram();
};

VLADLIB_NAMESPACE_CLOSE
