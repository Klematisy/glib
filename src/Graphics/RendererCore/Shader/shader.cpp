#include <fstream>
#include <filesystem>
#include <cstring>
#include <cassert>

#include "Graphics/GraphicsAPI/graphics_api.h"
#include "Graphics/RendererCore/renderer.h"
#include "preprocessor.h"
#include "shader.h"

#include "Logger/logger.h"

using namespace RendererCore;
using namespace GAPI;
static const auto gapi = rendererAPI;

std::string RendererCore::ParseFile(const std::string& filePath) {
    std::ifstream file(filePath);
    using namespace std::string_literals;
    std::string ch;

    if (!file.is_open()) {
        LOGERR("PARSER: File '"s + filePath + "' isn't open!");
        assert(1);
    }

    std::string result;
    while (getline(file, ch)) {
        result.append(ch + '\n');
    }

    return result;
}

std::string RendererCore::getShaderTypeInStr(GAPI::SHADER_TYPE type) {
    switch (type) {
        case GAPI::SHADER_TYPE::VERTEX:   return "VERTEX";   break;
        case GAPI::SHADER_TYPE::FRAGMENT: return "FRAGMENT"; break;
        case GAPI::SHADER_TYPE::COMPUTE:  return "COMPUTE";  break;
        case GAPI::SHADER_TYPE::GEOMETRY: return "GEOMETRY"; break;
    }
    return "";
}

Shader::Shader(std::string filePath, GAPI::SHADER_TYPE shaderType)
    : m_FilePath(filePath), m_ShaderType(shaderType)
{
    gapi->CreateShader(&m_Id, shaderType);
}

Shader::Shader(Shader&& other) noexcept {
    m_ShaderType = other.m_ShaderType;
    m_FilePath = other.m_FilePath;
    m_Id = other.m_Id;

    other.m_FilePath = "";
    other.m_Id = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    m_ShaderType = other.m_ShaderType;
    m_FilePath = other.m_FilePath;
    m_Id = other.m_Id;

    other.m_FilePath = "";
    other.m_Id = 0;
    return *this;
}

Shader::~Shader() {
    if (m_Id != 0)
        gapi->DeleteShader(&m_Id);
}

std::string ShaderCompiler::PreProcess(std::string src, std::string filePath) {
    PreProcessor pp;

    ParsedFile pf {src, filePath};
    pp.PreProcess(pf);

    src = pf.src;

#ifdef __GEN_PREPROCESSED_SHADER_SRC__
    using namespace std::string_literals;
    std::ofstream file("shader_cache/" + std::filesystem::path(filePath).filename().string());
    file << src;
    file.close();
#endif

    return src;
}

void ShaderCompiler::CheckShaderErrors(Shader& shader) {
    using namespace std::string_literals;

    int result;
    gapi->GetShaderiv(shader.m_Id, GAPI::SHADER_COMPILE::STATUS, &result);
    if (!result) {
        int length;
        gapi->GetShaderiv(shader.m_Id, GAPI::SHADER_COMPILE::INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length);
        gapi->GetShaderInfoLog(shader.m_Id, length, &length, message);
        LOGERR("SHADER: Failed to compile "s + getShaderTypeInStr(shader.m_ShaderType) + " shader!\nGLSL LOG:\n"s + message);
        free(message);
    }
}

void ShaderCompiler::Compile(Shader& shader) {
    std::string preProcessedShader = PreProcess(ParseFile(shader.m_FilePath), shader.m_FilePath);
    precompiledOptions.emplace_back(preProcessedShader);

    std::vector<const char*> options(precompiledOptions.size());
    for (uint32_t i = 0; i < options.size(); i++) {
        options[i] = precompiledOptions[i].c_str();
    }

    gapi->ShaderSource(shader.m_Id, options.size(), options.data(), nullptr);
    gapi->CompileShader(shader.m_Id);
    CheckShaderErrors(shader);

    precompiledOptions.pop_back();
}
