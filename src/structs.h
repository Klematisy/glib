#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace glib {
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
        glm::vec2 texCoords = glm::vec2(1.0f);
    };

    struct DrawResources {
        GlCore::ShaderProgram *shader = nullptr;
        GlCore::VertexArray vertexArray;
        GlCore::VertexBuffer vertexBuffer;
        GlCore::ElementBuffer elementBuffer;
    };
}