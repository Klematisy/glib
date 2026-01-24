#pragma once

#include "environment.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Graphics/RendererCore/window.h"

GLIB_NAMESPACE_OPEN

class Camera {
public:
    Camera(const RendererCore::Window* m_Window);
    Camera(glm::vec2 transition2, const RendererCore::Window* m_Window);
    Camera(glm::vec3 transition3, const RendererCore::Window* m_Window);

    void SetPosition(const glm::vec2& transition2);
    void SetPosition(const glm::vec3& transition3);
    void SetZoom(float zoom);
    void SetRotation(float rotation);

    virtual glm::mat4 GetVP() const;
    float GetZoom() const;
    float GetRotation() const;
protected:
    void UpdateView() const;
protected:
    const RendererCore::Window* m_Window = nullptr;

    mutable glm::mat4 m_View {1.0f};

    glm::vec3 m_Position = glm::vec3(0.0f);
    float m_Zoom = 0.0f;
    float m_Rotation = 0.0f;

    mutable bool m_NeedToUpdate = false;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const RendererCore::Window* m_Window);
    glm::mat4 GetVP() const override;
public:
    float aspectRatio = 1.0f;
    float zFar = 1000.0f;
    float zNear = 0.1f;
    float fov = 70.0f;
};


class OrthographicCamera : public Camera {
public:
    OrthographicCamera(const RendererCore::Window* m_Window);
    void SetRenderRange(float left, float right, float bottom, float top, float zNear = 0, float zFar = 1);
    glm::mat4 GetVP() const override;
private:
    glm::mat4 m_Ortho { 1.0f };
};

GLIB_NAMESPACE_CLOSE