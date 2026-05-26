#pragma once

#include "environment.h"
#include "glm/glm.hpp"

VLADLIB_NAMESPACE_OPEN

struct Rectangle {
    float x      = 0.0f;
    float y      = 0.0f;
    float width  = 0.0f;
    float height = 0.0f;
};

struct Quad {
    float x    = 0.0f;
    float y    = 0.0f;
    float size = 0.0f;
};

struct Color {
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};

struct Vertex {
   glm::vec3 pos {0};
   glm::vec4 col {0};
   glm::vec3 uv {0};
};


VLADLIB_NAMESPACE_CLOSE
