#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "OpenGLCore/Renderer.h"
#include "Window.h"
#include "Fonts/Font.h"

#include <utility>
#include <vector>
#include <functional>
#include <stack>

namespace glib {

    constexpr float epsilon = 0.0005f;

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
        float texSlot = 0.0f;
    };

    struct DrawResources {
        GlCore::ShaderProgram *shader = nullptr;
        GlCore::VertexArray vertexArray;
        GlCore::VertexBuffer vertexBuffer;
        GlCore::ElementBuffer elementBuffer;
        GlCore::Texture basicTexture;
    };

    class Shader {
    public:
        Shader() = default;
        Shader(const char *filePath);
        GlCore::ShaderProgram& GetShader();
    private:
        GlCore::ShaderProgram m_CustomShader;
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
        Camera() = default;
        explicit Camera(const GlCore::Window *m_Window);
        explicit Camera(glm::vec2 transition2, const GlCore::Window *m_Window);
        explicit Camera(glm::vec3 transition3, const GlCore::Window *m_Window);

        void UpdateView();

        void SetPosition(const glm::vec2& transition2);
        void SetPosition(const glm::vec3& transition3);
        void SetZoom(float zoom);
        void SetView(const glm::mat4& mat4);
        void SetRotation(float rotation);

        const glm::vec3& GetPosition();
        const glm::mat4& GetView();
        float GetZoom() const;
        float GetRotation() const;

    private:
        glm::mat4 m_View;
        const GlCore::Window *m_Window = nullptr;

        glm::vec3 m_Position = glm::vec3(0.0f);
        float m_Zoom = 0.0f;
        float m_Rotation = 0.0f;

        bool m_NeedToUpdate = false;
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
    public:
        CreateShape(GlCore::Window *window) : m_Window(window) {}
        CreateShape() = default;

        std::array<Vertex, 4>    Letter(float *x, float *y, const glm::vec2& midPoint, float angleInRadians, wchar_t symbol, LanguageTile& languageTile, int slot);
        std::array<Vertex, 4>      Rect(float x, float y, float width, float height, float angleD, Color color, int slot);
        std::array<Vertex, 4>   RectTex(float x, float y, float width, float height, float angleD, int slot);
        std::array<Vertex, 4>   RectTex(const Rectangle &objProperties, const Rectangle &texProperties, float angleD, int texWidth, int texHeight, int slot);

        static std::array<uint32_t , 6> RectangleIndices();

    private:
        GlCore::Window *m_Window = nullptr;
    };

    class Draw {
    public:
        explicit Draw(GlCore::Window &window);

        void Start();
        void End();

        void UseShader(Shader &shader);
        void UnUseShader();

        void UseFont(Font& font);
        void UnUseFont();

        Camera& GetCamera();

        const glm::mat4& GetProjMatrix() const;

        // angle in degrees

        void Text(const std::wstring& text, struct Quad quad, float angleD, Color color);
        void Text(const std::wstring& text, struct Quad quad,               Color color);

        void Rect(const Rectangle &rect,       float angleD, Color color);
        void Quad(const struct Quad &quad,     float angleD, Color color);

        void Texture(const Rectangle &rect,    float angleD, const GlCore::Texture *texture);
        void QTexture(const struct Quad &quad, float angleD, const GlCore::Texture *texture);

        void Texture(const Rectangle &objProperties, const Rectangle &texProperties, float angleD, const GlCore::Texture *texture);

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

        std::stack<GlCore::ShaderProgram*> m_ShaderStack;
        std::stack<const Font*> m_FontStack;

        Camera m_Camera;
        glm::mat4 m_Proj  = glm::mat4(1.0f);
        glm::mat4 m_Model = glm::mat4(1.0f);

        static constexpr uint32_t MINIMUM_SIZE = 1;
    };
}