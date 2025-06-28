#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "OpenGLCore/Renderer.h"
#include "Window.h"

#include <utility>
#include <vector>
#include <functional>

namespace glib {

    struct Rectangle {
        float x = 0.0f;
        float y = 0.0f;
        float width = 0.0f;
        float height = 0.0f;
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
        float texSlot = 0.0f;
    };

    struct DrawResources {
        GlCore::ShaderProgram shader;
        GlCore::VertexArray vertexArray;
        GlCore::VertexBuffer vertexBuffer;
        GlCore::ElementBuffer elementBuffer;
        GlCore::Texture basicTexture;
    };

    class Batch {
    public:
        Batch() = default;
        void BindDrawFunc(std::function<void()> func);
        void BatchClear();
        void BatchVertices(const Vertex* array, uint32_t size);
        void BatchIndices(const uint32_t* array, uint32_t size);

        void OverflowCheck();

        uint32_t GetVerticesSize();
        const void* GetVerticesData();

        uint32_t GetIndicesSize();
        const void* GetIndicesData();

        static uint32_t GetMaxBatch() ;
    private:
        std::function<void()> m_DrawBuffer;

        static constexpr uint32_t MAX_BATCH_SIZE = 10000;

        std::vector<Vertex>  m_Vertices;
        std::vector<uint32_t> m_Indices;

        uint32_t m_MaxIndex = 0;
    };


    class CreateShape {
    private:
        GlCore::Window *m_Window = nullptr;
    public:
        CreateShape(GlCore::Window *window) : m_Window(window) {}
        CreateShape() = default;

        std::array<Vertex, 4>      Rect(float x, float y, float width, float height, Color color, int slot);
        std::array<Vertex, 4>   RectTex(float x, float y, float width, float height, int slot);
        std::array<Vertex, 4>   RectTex(const Rectangle &objProperties, const Rectangle &texProperties, int texWidth, int texHeight, int slot);

        static std::array<uint32_t , 6> RectangleIndices();
    };


    class TextureSlotManager {
    public:
        TextureSlotManager();
        int PushTexture(const GlCore::Texture *texture);
        const int* GetSlotsData();
        int GetMaxSlotsCount() const;
        void Clear();
        void Bind();
        void BindDrawFunc(std::function<void()> DrawBuffer);
    private:
        std::function<void()> m_DrawBuffer;

        int m_MaxSlotsCount = 0;
        std::vector<const GlCore::Texture*> m_Textures;
        std::vector<int> m_Slots;
    };

    class Camera {
    public:
        explicit Camera();
        explicit Camera(glm::vec2 transition2);
        explicit Camera(glm::vec3 transition3);

        const glm::mat4& transition(const glm::vec2& transition2);
        const glm::mat4& transition(const glm::vec3& transition3);
        const glm::mat4& GetView();
        void SetView(const glm::mat4& mat4);
    private:
        glm::mat4 m_View;
    };

    class Draw {
    public:
        explicit Draw(GlCore::Window &window);

        void Start();
        void End();

        Camera& GetCamera();

        void Rect(float x, float y, float width, float height, Color color);
        void Quad(float x, float y, float size, Color color);

        void Texture(float x, float y, float width, float height, const GlCore::Texture *texture);
        void QTexture(float x, float y, float size, const GlCore::Texture *texture);

        void Texture(const Rectangle &objProperties, const Rectangle &texProperties, const GlCore::Texture *texture);
    private:
        void InitDrawResources();
        void DrawBuffer();
    private:
        GlCore::Window *m_Window = nullptr;
        GlCore::Renderer m_Renderer;

        DrawResources m_Gpu;

        TextureSlotManager m_TSlotManager;

        Batch m_Batch;
        CreateShape m_CreateShape;

        Camera m_Camera;
        glm::mat4 m_Proj = glm::mat4(1.0f);
    };
}