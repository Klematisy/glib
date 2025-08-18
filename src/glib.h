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

#include "structs.h"
#include "graphicsUtils.h"

namespace glib {

    constexpr float epsilon = 0.0005f;

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

    class CreateShape {
    public:
        CreateShape(GlCore::Window *window) : m_Window(window) {}
        CreateShape() = default;

        std::array<Vertex, 4>    Letter(float *x, float *y, const glm::vec2& midPoint, float angleInRadians, wchar_t symbol, LanguageTile& languageTile, const TexInfo& tex);
        std::array<Vertex, 4>      Rect(float x, float y, float width, float height, float angleD, Color color, const TexInfo& tex);
        std::array<Vertex, 4>   RectTex(float x, float y, float width, float height, float angleD, const TexInfo& tex);
        std::array<Vertex, 4>   RectTex(const Rectangle &objProperties, const Rectangle &texProperties, float angleD, const TexInfo& tex);

        static std::array<uint32_t , 6> RectangleIndices();
        static glm::vec2 GetTextCenter(const std::wstring& text, struct Quad quad, const std::vector<LanguageTileSet>& tileSet);

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

        void Texture(const Rectangle &rect,    float angleD, const class Texture *texture);
        void QTexture(const struct Quad &quad, float angleD, const class Texture *texture);

        void Texture(const Rectangle &objProperties, const Rectangle &texProperties, float angleD, const class Texture *texture);
        TextureManager m_TexManager;
    private:
        void InitDrawResources();
        void DrawBuffer();
    private:
        GlCore::Window *m_Window = nullptr;
        GlCore::Renderer m_Renderer;

        DrawResources m_Gpu;


        uint32_t m_ChosenSlot = 0;
        const class Texture *m_BasicTexture;

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