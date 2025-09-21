#pragma once

#include "environment.h"

GLIB_NAMESPACE_OPEN

class Shader {
public:
    Shader() = default;
    Shader(const char *filePath);
    GlCore::ShaderProgram& GetShader();
private:
    GlCore::ShaderProgram m_CustomShader;
};

GLIB_NAMESPACE_CLOSE