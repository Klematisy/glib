#include "shader.h"
#include "GraphicsAPI/graphics_api.h"
#include <cstring>
#include <memory>

VLADLIB_NAMESPACE_USING;

u0 Shader::Init() {
    m_Program = GAPI::createShaderProgram();
    m_Compiler = GAPI::createShaderCompiler();
    m_Compiler->precompiledOptions.resize(2);
    m_Compiler->precompiledOptions[0] = GAPI::getShaderLanguageVersion();
}

void Shader::AddSrcFile(const char* filePath, GAPI::SHADER_TYPE types) {
    m_Shaders.emplace_back(filePath, types);
    m_AddedCount++;
}

void Shader::HotReload() {
    m_Program = GAPI::createShaderProgram();
    m_AddedCount = m_Shaders.size();
    Compile();
}

void Shader::Compile() {
    if (m_AddedCount == 0) return;

    std::vector<GAPI::ShaderPTR> shader;

    auto compileShader = [&](GAPI::SHADER_TYPE stype, uint32_t i) {
        shader.emplace_back(createShader(m_Shaders[i].first, stype));
        std::string define = "#define __" + GAPI::getShaderTypeInStr(stype) + "_SHADER_TYPE__\n";
        m_Compiler->precompiledOptions[1] = define;
        m_Compiler->Compile(shader.back().get());
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
        m_Program->AttachShader(s.get());
    }

    m_Program->LinkProgram();
}

GAPI::ShaderProgram* Shader::GetShaderProgram() const {
    return m_Program.get();
}
