#pragma once

#include "glm/glm.hpp"
#include "environment.h"

#define GEOM_NAMESPACE_OPEN  namespace Geom {
#define GEOM_NAMESPACE_CLOSE }

VLADLIB_NAMESPACE_OPEN
GEOM_NAMESPACE_OPEN

struct Transform {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation {0};
    glm::vec3 scale {1};
    glm::vec3 deltaPivot {0};
};

GEOM_NAMESPACE_CLOSE
VLADLIB_NAMESPACE_CLOSE
