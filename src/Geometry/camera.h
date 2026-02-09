#pragma once

#include "environment.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Graphics/RendererCore/window.h"

GLIB_NAMESPACE_OPEN

class Camera {
public:
    Camera();
    Camera(glm::vec2 transition2);
    Camera(glm::vec3 transition3);

    void SetPosition(const glm::vec3& transition3);
    void SetRotation(const glm::vec3& rotation);

    virtual glm::mat4 GetVP() const;
    glm::vec3 GetPosition() const;
    const glm::vec3& GetRotation() const;
    virtual glm::mat4 GetProject() const;
    glm::mat4 GetView() const;
protected:
    void UpdateView() const;
protected:
    mutable glm::mat4 m_View {1.0f};

    glm::vec3 m_Position = glm::vec3(0.0f);
    glm::vec3 m_Rotation = glm::vec3(0.0f);

    mutable bool m_NeedToUpdate = false;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera() = default;
    glm::mat4 GetVP() const override;
    glm::mat4 GetProject() const override;
public:
    float aspectRatio = 1.0f;
    float zFar = 1000.0f;
    float zNear = 0.1f;
    float fov = 70.0f;
};


class OrthographicCamera : public Camera {
public:
    OrthographicCamera() = default;
    void SetRenderRange(float left, float right, float bottom, float top, float zNear = 0, float zFar = 1);
    glm::mat4 GetVP() const override;
    glm::mat4 GetProject() const override;
private:
    glm::mat4 m_Ortho { 1.0f };
};

GLIB_NAMESPACE_CLOSE