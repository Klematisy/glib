#include <iostream>

#include "ShaderProgram.h"

int GlCore::ShaderProgram::LoadFromFile(const char* filePath) {
    std::string ShaderSourceCode = ShaderSourceLoader::Parse(filePath);
    if (CreateShaderProgram(ShaderSourceCode) == -1) {
        std::cerr << "SHADER: '" << filePath << "' hasn't loaded" << std::endl;
        return -1;
    }

    std::cout << "SHADER: '" << filePath << "' has loaded" << std::endl;
    return 0;
}

int GlCore::ShaderProgram::LoadFromString(const std::string& SourceCode) {
    return CreateShaderProgram(SourceCode);
}

int GlCore::ShaderProgram::CreateShaderProgram(const std::string& ShaderSourceCode) {
    if (m_ShaderProgram != 0)
        glDeleteProgram(m_ShaderProgram);

    m_ShaderProgram = glCreateProgram();

    int VertexShader   = ShaderCreator::CreateShader(ShaderSourceCode, GL_VERTEX_SHADER);
    int FragmentShader = ShaderCreator::CreateShader(ShaderSourceCode, GL_FRAGMENT_SHADER);

    glAttachShader(m_ShaderProgram, VertexShader);
    glAttachShader(m_ShaderProgram, FragmentShader);
    glLinkProgram(m_ShaderProgram);

//    glValidateProgram(m_ShaderProgram);

    ShaderCreator::CheckLinkingErrors(m_ShaderProgram);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    return (VertexShader == -1 || FragmentShader == -1) ? -1 : 0;
}

GlCore::ShaderProgram& GlCore::ShaderProgram::operator=(GlCore::ShaderProgram &&other) {
    m_ShaderProgram = other.m_ShaderProgram;
    UniformLocations = std::move(UniformLocations);

    other.m_ShaderProgram = 0;

    return *this;
}

GlCore::ShaderProgram &GlCore::ShaderProgram::operator=(const GlCore::ShaderProgram &other) {
    m_ShaderProgram = other.m_ShaderProgram;
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

void GlCore::ShaderProgram::SetUniform1f(const std::string &name, float value) {
    glUniform1f(GetUniformLocation(name), value);
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
