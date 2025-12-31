#pragma once

#include "environment.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Graphics/RendererCore/window.h"

GLIB_NAMESPACE_OPEN

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

//struct Vertex {
//    glm::vec3 position;
//    glm::vec4 color;
//    glm::vec3 texCoords;
//};


GLIB_NAMESPACE_CLOSE