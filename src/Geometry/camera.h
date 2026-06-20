#pragma once

#include "environment.h"
#include "transform.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GraphicsAPI/window.h"

VLADLIB_NAMESPACE_OPEN

class Camera {
public:
    Camera() = default;

    virtual glm::mat4 GetVP() const;
    virtual glm::mat4 GetProject() const;
    glm::mat4 GetView() const;

    float zNear = 0.1f;
    float zFar = 1000.0f;
    Geom::Transform transform;
protected:
    glm::mat4 CalculateView() const;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera();
    glm::mat4 GetVP() const override;
    glm::mat4 GetProject() const override;

    float aspectRatio = 1.0f;
    float fov = 70.0f;
};


class OrthographicCamera : public Camera {
public:
    OrthographicCamera();
    glm::mat4 GetVP() const override;
    glm::mat4 GetProject() const override;

    float left = 0;
    float right = 0;
    float bottom = 0;
    float top = 0;
};

VLADLIB_NAMESPACE_CLOSE