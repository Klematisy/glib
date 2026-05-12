#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

#include "DrawUtils/texture_atlas.h"
#include "DrawUtils/shader.h"

#include "glm/glm.hpp"

#define GEOM_NAMESPACE_OPEN  namespace Geom {
#define GEOM_NAMESPACE_CLOSE }

VLADLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

struct Material {
    const RendererCore::ImageInfo* image = nullptr;
    const Shader* shader = nullptr;

    std::vector<glm::vec4> colors;
    std::vector<glm::vec2> uvCoordinates;
};

GEOM_NAMESPACE_CLOSE
VLADLIB_NAMESPACE_CLOSE
