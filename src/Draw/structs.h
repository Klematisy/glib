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

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 texCoords;
};

struct DrawBuffer {
    std::shared_ptr<RendererCore::VertexArray> vertexArray;
    std::shared_ptr<RendererCore::VertexBuffer> vertexBuffer;
    std::shared_ptr<RendererCore::ElementBuffer> elementBuffer;
};

inline DrawBuffer CreateDrawBasicsResources() {
    DrawBuffer dr;

    dr.vertexArray = std::make_shared<RendererCore::VertexArray>();
    dr.vertexBuffer = std::make_shared<RendererCore::VertexBuffer>(GAPI::DRAW_TYPE::DYNAMIC, 0, nullptr);
    dr.elementBuffer = std::make_shared<RendererCore::ElementBuffer>(GAPI::DRAW_TYPE::DYNAMIC, 0, nullptr);

    RendererCore::VertexArrayLayout layout;
    layout.Add<float>(3);
    layout.Add<float>(4);
    layout.Add<float>(3);
    dr.vertexArray->AddBuffer(layout, *dr.vertexBuffer);

    dr.vertexArray->UnBind();
    dr.vertexBuffer->UnBind();
    dr.elementBuffer->UnBind();

    return dr;
}

GLIB_NAMESPACE_CLOSE