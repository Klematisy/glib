#include "shader.h"
#include "Graphics/GraphicsAPI/graphics_api.h"
#include "Graphics/RendererCore/Shader/shader.h"
#include "Graphics/RendererCore/renderer.h"
#include <cstring>

VLADLIB_NAMESPACE_USING;

namespace rc = RendererCore;

using glcore_sp = rc::ShaderProgram;

Shader::Shader() {
    m_Compiler.precompiledOptions.resize(2);
    m_Compiler.precompiledOptions[0] = rc::rendererAPI->GetShaderLanguageVersion(rc::rendererContext) + '\n';
}

void Shader::AddSrcFile(const char* filePath, SHADER_TYPE types) {
    m_Shaders.emplace_back(filePath, types);
    m_AddedCount++;
}

void Shader::HotReload() {
    m_Program = std::make_shared<rc::ShaderProgram>();
    m_AddedCount = m_Shaders.size();
    Compile();
}

void Shader::Compile() {
    if (m_AddedCount == 0) return;

    std::vector<rc::Shader> shader;

    auto compileShader = [&](GAPI::SHADER_TYPE stype, uint32_t i) {
        shader.emplace_back(m_Shaders[i].first, stype);
        std::string define = "#define __" + rc::getShaderTypeInStr(stype) + "_SHADER_TYPE__\n";
        m_Compiler.precompiledOptions[1] = define;
        m_Compiler.Compile(shader.back());
    };

    uint32_t size = m_Shaders.size();
    for (uint32_t i = size - m_AddedCount; i < size; i++) {
        if ((m_Shaders[i].second & GAPI::SHADER_TYPE::VERTEX) != 0) {
            compileShader(GAPI::SHADER_TYPE::VERTEX, i);
        }

        if ((m_Shaders[i].second & GAPI::SHADER_TYPE::FRAGMENT) != 0) {
            compileShader(GAPI::SHADER_TYPE::FRAGMENT, i);
        }

        if ((m_Shaders[i].second & GAPI::SHADER_TYPE::COMPUTE) != 0) {
            compileShader(GAPI::SHADER_TYPE::COMPUTE, i);
        }

        if ((m_Shaders[i].second & GAPI::SHADER_TYPE::GEOMETRY) != 0) {
            compileShader(GAPI::SHADER_TYPE::GEOMETRY, i);
        }
    }

    for (auto& s : shader) {
        m_Program->AttachShader(s);
    }

    m_Program->LinkProgram();
}

glcore_sp* Shader::GetShaderProgram() const {
    return m_Program.get();
}
