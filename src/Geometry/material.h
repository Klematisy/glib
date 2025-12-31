#pragma once

#include <iostream>
#include <vector>

#include "Graphics/RendererCore/Shader/shader_program.h"
#include "glm/glm.hpp"

GLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

struct Material {
    std::vector<float> uvCoordinates;
    std::vector<glm::vec4> colors;
    RendererCore::ShaderProgram* shader = nullptr;
};

GLIB_NAMESPACE_CLOSE
GEOM_NAMESPACE_CLOSE