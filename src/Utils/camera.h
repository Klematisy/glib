#pragma once

#include "environment.h"

GLIB_NAMESPACE_OPEN

class Camera {
public:
    explicit Camera(const RendererCore::Window* m_Window);
    explicit Camera(glm::vec2 transition2, const RendererCore::Window* m_Window);
    explicit Camera(glm::vec3 transition3, const RendererCore::Window* m_Window);

    void UpdateView() const;

    void SetPosition(const glm::vec2& transition2);
    void SetPosition(const glm::vec3& transition3);
    void SetZoom(float zoom);
    void SetView(const glm::mat4& mat4);
    void SetRotation(float rotation);

    const glm::vec3& GetPosition() const;
    const glm::mat4& GetView() const;
    float GetZoom() const;
    float GetRotation() const;
private:
    const RendererCore::Window* m_Window = nullptr;

    mutable glm::mat4 m_View {1.0f};
    glm::vec3 m_Position = glm::vec3(0.0f);
    float m_Zoom = 0.0f;
    float m_Rotation = 0.0f;

    mutable bool m_NeedToUpdate = false;
};

GLIB_NAMESPACE_CLOSE