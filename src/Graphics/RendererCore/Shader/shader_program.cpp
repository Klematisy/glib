#include <vector>
#include <cassert>

#include "Graphics/RendererCore/renderer.h"
#include "Logger/logger.h"
#include "shader_program.h"

using namespace RendererCore;
using namespace GAPI;

static const auto gapi = rendererAPI;

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) {
    m_ShaderProgram = other.m_ShaderProgram;
    m_AttachedShaders = std::move(other.m_AttachedShaders);
    m_UniformLocations = std::move(other.m_UniformLocations);

    other.m_ShaderProgram = 0;

    return *this;
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram& other) {
    m_ShaderProgram = other.m_ShaderProgram;
    m_AttachedShaders = other.m_AttachedShaders;
    m_UniformLocations = other.m_UniformLocations;

    return *this;
}

ShaderProgram::ShaderProgram() {
    gapi->CreateProgram(&m_ShaderProgram);
}

ShaderProgram::~ShaderProgram() {
    UnBind();
    gapi->DeleteProgram(&m_ShaderProgram);
}

void ShaderProgram::LinkProgram() {
    if (m_ShaderProgram != 0) {
        UnBind();
        gapi->DeleteProgram(&m_ShaderProgram);
        gapi->CreateProgram(&m_ShaderProgram);
    }

    for (auto& shader : m_AttachedShaders) {
        if (shader) {
            gapi->AttachShader(m_ShaderProgram, shader->m_Id);
        } else {
            LOGERR("Shader Linker: shader is dead!");
        }
    }

    gapi->LinkProgram(m_ShaderProgram);
    CheckLinkingErrors();
}

void ShaderProgram::CheckLinkingErrors() const {
    int result;
    gapi->GetProgramiv(m_ShaderProgram, GAPI::SHADER_PROGRAM_COMPILE::LINK_STATUS, &result);
    if (!result) {
        int length;
        gapi->GetProgramiv(m_ShaderProgram, GAPI::SHADER_PROGRAM_COMPILE::INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        gapi->GetProgramInfoLog(m_ShaderProgram, length, nullptr, message);
        LOGERR("SHADER PROGRAM: Failed to link program!");
        LOGERR(message);
        free(message);
    }
}

void ShaderProgram::AttachShader(const Shader& shader) {
    m_AttachedShaders.push_back(&shader);
}

void ShaderProgram::Bind() const {
    if (m_ShaderProgram == 0)
        LOGWARN("SHADER PROGRAM: This sp doesn't exist!"); // TODO: improve this log in a future

    assert(m_ShaderProgram != 0);

    gapi->UseProgram(m_ShaderProgram);
}

void ShaderProgram::UnBind() const {
    gapi->UseProgram(0);
}

void ShaderProgram::ClearShaders() {
    m_AttachedShaders.clear();
}




int ShaderProgram::GetUniformLocation(const std::string& name) const {
    if (m_UniformLocations.find(name) == m_UniformLocations.end())
        m_UniformLocations[name] = gapi->GetUniformLocation(m_ShaderProgram, name.c_str());

    return m_UniformLocations[name];
}

void ShaderProgram::SetInt(const std::string& name, int value) const {
    Bind();
    gapi->Uniform1i(GetUniformLocation(name), value);
    UnBind();
}

void ShaderProgram::SetFloat(const std::string& name, float value) const {
    Bind();
    gapi->Uniform1f(GetUniformLocation(name), value);
    UnBind();
}

void ShaderProgram::SetFloat2(const std::string& name, const glm::vec2& value) const {
    Bind();
    gapi->Uniform2f(GetUniformLocation(name), value.x, value.y);
    UnBind();
}

void ShaderProgram::SetIntArray(const std::string& name, uint32_t count, const int* value) const {
    Bind();
    gapi->Uniform1iv(GetUniformLocation(name), count, value);
    UnBind();
}

void ShaderProgram::SetMatrixFloat4(const std::string& name, const float* value_ptr) const {
    Bind();
    gapi->UniformMatrix4fv(GetUniformLocation(name), 1, GAPI::API_BOOLEAN::FALSE, value_ptr);
    UnBind();
}
