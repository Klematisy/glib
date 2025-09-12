#pragma once

#include "environment.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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

struct Vertex {
    glm::vec3 position;
    Color color;
    glm::vec3 texCoords;
};

struct DrawResources {
    GlCore::ShaderProgram *shader = nullptr;
    GlCore::VertexArray vertexArray;
    GlCore::VertexBuffer vertexBuffer;
    GlCore::ElementBuffer elementBuffer;
};

GLIB_NAMESPACE_CLOSE