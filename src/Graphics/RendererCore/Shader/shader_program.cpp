#include <vector>
#include <cassert>

#include "Logger/logger.h"
#include "shader_program.h"

using namespace RendererCore;
using namespace GAPI;

static auto& gapi = GraphicsAPIImpl::Get();

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) {
    m_ShaderProgram = other.m_ShaderProgram;
    m_AttachedShaders = std::move(other.m_AttachedShaders);
    UniformLocations = std::move(other.UniformLocations);

    other.m_ShaderProgram = 0;

    return *this;
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& other) {
    m_ShaderProgram = other.m_ShaderProgram;
    m_AttachedShaders = other.m_AttachedShaders;
    UniformLocations = other.UniformLocations;

    return *this;
}

void ShaderProgram::CreateProgram() {
    if (m_ShaderProgram != 0) {
        UnBind();
        gapi.DeleteProgram(&m_ShaderProgram);
    }

    m_ShaderProgram = gapi.CreateProgram();

    for (uint32_t shader : m_AttachedShaders)
        gapi.AttachShader(m_ShaderProgram, shader);

    gapi.LinkProgram(m_ShaderProgram);

    CheckLinkingErrors();
}

void ShaderProgram::CheckLinkingErrors() const {
    int result;
    gapi.GetProgramiv(m_ShaderProgram, GAPI::SHADER_PROGRAM_COMPILE::LINK_STATUS, &result);
    if (!result) {
        int length;
        gapi.GetProgramiv(m_ShaderProgram, GAPI::SHADER_PROGRAM_COMPILE::INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        gapi.GetProgramInfoLog(m_ShaderProgram, length, nullptr, message);
        Logger::LogErr("SHADER PROGRAM", "\nFailed to link program!");
        Logger::LogErr("SHADER PROGRAM", message);
        free(message);
    }
}

void ShaderProgram::AttachShader(const Shader& shader) {
    auto vec = shader.GetShaders();
    for (uint32_t shaderId : vec) {
        m_AttachedShaders.push_back(shaderId);
    }
}

void ShaderProgram::Bind() const {
    if (m_ShaderProgram == 0)
        Logger::LogWar("SHADER PROGRAM", "This sp doesn't exist!"); // TODO: improve this log in a future

    assert(m_ShaderProgram != 0);

    gapi.UseProgram(m_ShaderProgram);
}

void ShaderProgram::UnBind() const {
    gapi.UseProgram(0);
}

void ShaderProgram::ClearShaders() {
    m_AttachedShaders.clear();
}






int ShaderProgram::GetUniformLocation(const std::string& name) {
    if (UniformLocations.find(name) == UniformLocations.end())
        UniformLocations[name] = gapi.GetUniformLocation(m_ShaderProgram, name.c_str());

    return UniformLocations[name];
}

void ShaderProgram::SetInt(const std::string& name, int value) {
    Bind();
    gapi.Uniform1i(GetUniformLocation(name), value);
    UnBind();
}

void ShaderProgram::SetFloat(const std::string& name, float value) {
    Bind();
    gapi.Uniform1f(GetUniformLocation(name), value);
    UnBind();
}

void ShaderProgram::SetIntArray(const std::string& name, uint32_t count, const int* value) {
    Bind();
    gapi.Uniform1iv(GetUniformLocation(name), count, value);
    UnBind();
}

void ShaderProgram::SetMatrixFloat4(const std::string& name, const float* value_ptr) {
    Bind();
    gapi.UniformMatrix4fv(GetUniformLocation(name), 1, GAPI::API_BOOLEAN::FALSE, value_ptr);
    UnBind();
}

ShaderProgram::~ShaderProgram() {
    UnBind();
    gapi.DeleteProgram(&m_ShaderProgram);
}
