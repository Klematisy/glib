#include <iostream>

#include "ShaderProgram.h"

GlCore::ShaderProgram::ShaderProgram(const std::string &filePath) {
    CreateShaderProgram(filePath);
}

void GlCore::ShaderProgram::CreateShaderProgram(const std::string &filePath) {
    ShaderCreator sc(filePath);

    m_ShaderProgram = glCreateProgram();

    uint32_t VertexShader   = sc.CreateShader(GL_VERTEX_SHADER);
    uint32_t FragmentShader = sc.CreateShader(GL_FRAGMENT_SHADER);

    glAttachShader(m_ShaderProgram, VertexShader);
    glAttachShader(m_ShaderProgram, FragmentShader);
    glLinkProgram(m_ShaderProgram);

//    glValidateProgram(m_ShaderProgram);

    ShaderCreator::CheckLinkingErrors(m_ShaderProgram);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

GlCore::ShaderProgram& GlCore::ShaderProgram::operator=(GlCore::ShaderProgram &&other) {
    m_ShaderProgram = other.m_ShaderProgram;
    UniformLocations = std::move(UniformLocations);

    other.m_ShaderProgram = 0;

    return *this;
}

void GlCore::ShaderProgram::Bind() const {
    glUseProgram(m_ShaderProgram);
}

void GlCore::ShaderProgram::UnBind() const {
    glUseProgram(0);
}

int GlCore::ShaderProgram::GetUniformLocation(const std::string &name) {
    if (UniformLocations.find(name) == UniformLocations.end()) {
        UniformLocations[name] = glGetUniformLocation(m_ShaderProgram, name.c_str());
    }

    return UniformLocations[name];
}

void GlCore::ShaderProgram::SetUniform1i(const std::string &name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void GlCore::ShaderProgram::SetUniform1iv(const std::string &name, uint32_t count, const int *value) {
    glUniform1iv(GetUniformLocation(name), count, value);
}

void GlCore::ShaderProgram::SetUniformMatrix4fv(const std::string &name, const float *value_ptr) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value_ptr);
}

GlCore::ShaderProgram::~ShaderProgram() {
    UnBind();
    if (m_ShaderProgram != 0)
        glDeleteProgram(m_ShaderProgram);
}
