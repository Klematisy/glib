#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "OpenGLCore/Renderer.h"
#include "Window.h"

#include <vector>

namespace glib {
    using namespace GlCore;

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
        float texSlot = 0.0f;
    };

    struct DrawResources {
        ShaderProgram shader;
        VertexArray vertexArray;
        VertexBuffer vertexBuffer;
        ElementBuffer elementBuffer;
        Texture basicTexture;
    };

    class Batch {
    public:
        Batch() = default;
        void BatchClear();
        void BatchVertices(const Vertex* array, uint32_t size);
        void BatchIndices(const uint32_t* array, uint32_t size);

        uint32_t GetVerticesSize();
        const void* GetVerticesData();

        uint32_t GetIndicesSize();
        const void* GetIndicesData();
    private:
        std::vector<Vertex>  m_Vertices;
        std::vector<uint32_t> m_Indices;

        uint32_t m_MaxIndex = 0;
    };

    class Draw {
    public:
        explicit Draw(Window &window);

        void Start();
        void End();

        void Rect(float x, float y, float width, float height, Color color);
        void Quad(float x, float y, float size, Color color);

    private:

    private:
        Window *m_Window = nullptr;
        Renderer m_Renderer;

        DrawResources m_Gpu;

        Batch m_Batch;

        glm::mat4 m_Proj = glm::mat4(1.0f);
    };

    class CreateShape {
    public:
        static std::array<Vertex, 4> Rectangle(float x, float y, float width, float height, Color color);
        static std::array<uint32_t , 6> RectangleIndices();
    };
}