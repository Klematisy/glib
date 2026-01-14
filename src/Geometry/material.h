#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>

#include "Graphics/RendererCore/Shader/shader_program.h"

#include "glm/glm.hpp"

GLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

struct Material {
    const RendererCore::ImageInfo* texture = nullptr;
    RendererCore::TextureParameters textureParameters;

    RendererCore::ShaderProgram* shader = nullptr;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec2> uvCoordinates;
};

GLIB_NAMESPACE_CLOSE
GEOM_NAMESPACE_CLOSE