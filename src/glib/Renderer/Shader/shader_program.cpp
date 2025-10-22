#include <vector>

#include "Logger/logger.h"
#include "shader_program.h"

using namespace GlCore;

ShaderProgram& ShaderProgram::operator=(ShaderProgram &&other) {
    m_ShaderProgram = other.m_ShaderProgram;
    m_AttachedShaders = std::move(other.m_AttachedShaders);
    UniformLocations = std::move(other.UniformLocations);

    other.m_ShaderProgram = 0;

    return *this;
}

ShaderProgram& ShaderProgram::operator=(const ShaderProgram &other) {
    m_ShaderProgram = other.m_ShaderProgram;
    m_AttachedShaders = other.m_AttachedShaders;
    UniformLocations = other.UniformLocations;

    return *this;
}

void ShaderProgram::CreateProgram() {
    if (m_ShaderProgram != 0) {
        UnBind();
        glDeleteProgram(m_ShaderProgram);
    }

    m_ShaderProgram = glCreateProgram();

    for (uint32_t shader : m_AttachedShaders)
        glAttachShader(m_ShaderProgram, shader);

    glLinkProgram(m_ShaderProgram);

    CheckLinkingErrors();
}

void ShaderProgram::CheckLinkingErrors() const {
    int result;
    glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetProgramInfoLog(m_ShaderProgram, length, nullptr, message);
        Logger::LogErr("SHADER PROGRAM", "\nFailed to link program!");
        Logger::LogErr("SHADER PROGRAM", message);
        free(message);
    }
}

void ShaderProgram::AttachShader(const Shader &shader) {
    auto vec = shader.GetShaders();
    for (uint32_t shaderId : vec) {
        m_AttachedShaders.push_back(shaderId);
    }
}

void ShaderProgram::Bind() const {
    if (m_ShaderProgram == 0)
        Logger::LogWar("SHADER PROGRAM", "This sp doesn't exist!"); // TODO: improve this log in a future

    glUseProgram(m_ShaderProgram);
}

void ShaderProgram::UnBind() const {
    glUseProgram(0);
}

void ShaderProgram::ClearShaders() {
    m_AttachedShaders.clear();
}






int ShaderProgram::GetUniformLocation(const std::string &name) {
    if (UniformLocations.find(name) == UniformLocations.end())
        UniformLocations[name] = glGetUniformLocation(m_ShaderProgram, name.c_str());

    return UniformLocations[name];
}

void ShaderProgram::SetUniform1i(const std::string &name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform1f(const std::string &name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void ShaderProgram::SetUniform1iv(const std::string &name, uint32_t count, const int *value) {
    glUniform1iv(GetUniformLocation(name), count, value);
}

void ShaderProgram::SetUniformMatrix4fv(const std::string &name, const float *value_ptr) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value_ptr);
}

ShaderProgram::~ShaderProgram() {
    UnBind();
    if (m_ShaderProgram != 0)
        glDeleteProgram(m_ShaderProgram);
}
