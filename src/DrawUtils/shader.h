#pragma once

#include <vector>
#include <memory>
#include <cstring>

#include "Graphics/GraphicsAPI/graphics_api.h"
#include "Graphics/RendererCore/Shader/shader_program.h"
#include "environment.h"

VLADLIB_NAMESPACE_OPEN

class Shader {
    using glcore_sp = RendererCore::ShaderProgram;
public:
    Shader();

    void AddSrcFile(const char* filePath, SHADER_TYPE types);
    void Compile();
    void HotReload();
    bool IsValid() const;
    glcore_sp* GetShaderProgram() const;
private:
    uint32_t m_AddedCount = 0;
    RendererCore::ShaderCompiler m_Compiler;

    std::vector<std::pair<const char*, SHADER_TYPE>> m_Shaders;
    std::shared_ptr<glcore_sp> m_Program = std::make_shared<glcore_sp>();
};

VLADLIB_NAMESPACE_CLOSE
