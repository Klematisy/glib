#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Graphics/RendererCore/Shader/shader_program.h"
#include "DrawUtils/texture_atlas.h"

#include "glm/glm.hpp"

GLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

struct Material {
    const RendererCore::ImageInfo* image = nullptr;

    std::shared_ptr<RendererCore::ShaderProgram> shader;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec2> uvCoordinates;
};

GLIB_NAMESPACE_CLOSE
GEOM_NAMESPACE_CLOSE