#include "camera.h"

GLIB_NAMESPACE_OPEN

Camera::Camera()
    : m_View(glm::mat4(1.0f))
{}

Camera::Camera(glm::vec2 transition2)
    : m_View(glm::mat4(1.0f))
{
    m_Position = {transition2, 0.0f};
}

Camera::Camera(glm::vec3 transition3)
    : m_View(glm::mat4(1.0f))
{
    m_Position = transition3;
}

void Camera::SetPosition(const glm::vec3& pos3) {
    m_Position = -pos3;
    m_NeedToUpdate = true;
}

void Camera::SetRotation(const glm::vec3& rotation) {
    m_Rotation = rotation;
    m_NeedToUpdate = true;
}

void Camera::UpdateView() const {
    m_View = glm::mat4(1.0f);

    m_View = glm::rotate(m_View, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_View = glm::rotate(m_View, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_View = glm::rotate(m_View, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    m_View = glm::translate(m_View, m_Position);

    m_NeedToUpdate = false;
}

glm::mat4 Camera::GetVP() const {
    if (m_NeedToUpdate) UpdateView();
    return m_View;
}

const glm::vec3& Camera::GetRotation() const {
    return m_Rotation;
}

glm::mat4 Camera::GetView() const {
    if (m_NeedToUpdate) UpdateView();
    return m_View;
}

glm::mat4 Camera::GetProject() const { return glm::mat4(1.0f); }

glm::vec3 Camera::GetPosition() const {
    return -m_Position;
}


glm::mat4 PerspectiveCamera::GetVP() const {
    if (m_NeedToUpdate) UpdateView();
    return glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar) * m_View;
}

glm::mat4 PerspectiveCamera::GetProject() const {
    return glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
}

void OrthographicCamera::SetRenderRange(float left, float right, float bottom, float top, float zNear /* = 0*/, float zFar /* = 1*/) {
    m_Ortho = glm::ortho(left, right, bottom, top, zNear, zFar);
}

glm::mat4 OrthographicCamera::GetVP() const {
    if (m_NeedToUpdate) UpdateView();
    return m_Ortho * m_View;
}

glm::mat4 OrthographicCamera::GetProject() const {
    return m_Ortho;
}


GLIB_NAMESPACE_CLOSE

