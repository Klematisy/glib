#pragma once

#include <vector>
#include <cstring>

#include "GraphicsAPI/graphics_api.h"
#include "environment.h"

VLADLIB_NAMESPACE_OPEN

class Shader {
public:
    u0 Init();

    void AddSrcFile(const char* filePath, GAPI::SHADER_TYPE types);
    void Compile();
    void HotReload();
    bool IsValid() const;
    GAPI::ShaderProgram* GetShaderProgram() const;
private:
    uint32_t m_AddedCount = 0;
    GAPI::ShaderCompilerPTR m_Compiler = nullptr;

    std::vector<std::pair<const char*, GAPI::SHADER_TYPE>> m_Shaders;
    GAPI::ShaderProgramPTR m_Program;
};

VLADLIB_NAMESPACE_CLOSE
