#include "draw.h"

#include "camera.h"

GLIB_NAMESPACE_OPEN

Camera::Camera(const GlCore::Window *m_Window)
    : m_View(glm::mat4(1.0f)), m_Window(m_Window)
{}

Camera::Camera(glm::vec2 transition2, const GlCore::Window *m_Window)
    : m_View(glm::mat4(1.0f)), m_Window(m_Window)
{
    m_Position = {transition2, 0.0f};
}

Camera::Camera(glm::vec3 transition3, const GlCore::Window *m_Window)
    : m_View(glm::mat4(1.0f)), m_Window(m_Window)
{
    m_Position = transition3;
}

const glm::mat4 &Camera::GetView() {
    if (m_NeedToUpdate) UpdateView();
    return m_View;
}

const glm::vec3 &Camera::GetPosition() {
    return m_Position;
}

float Camera::GetZoom() const {
    return m_Zoom;
}

void Camera::SetView(const glm::mat4& mat4) {
    m_View = mat4;
}

void Camera::SetPosition(const glm::vec2& pos2) {
    m_Position = { pos2, m_Position.z };
    m_NeedToUpdate = true;
}

void Camera::SetPosition(const glm::vec3& pos3) {
    m_Position = pos3;
    m_NeedToUpdate = true;
}

void Camera::SetZoom(float zoom) {
    m_Zoom = zoom;
    m_NeedToUpdate = true;
}

void Camera::SetRotation(float rotation) {
    m_Rotation = glm::radians(rotation);
    m_NeedToUpdate = true;
}

void Camera::UpdateView() {
    glm::vec2 center(m_Window->GetWidth() / 2, m_Window->GetHeight() / 2);

    m_View = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f));

    m_View = glm::scale(m_View, glm::vec3(m_Zoom, m_Zoom, 1.0f));

    m_View = glm::rotate(m_View, m_Rotation, glm::vec3(0.0f, 0.0f, 1.0f));

    m_View = glm::translate(m_View, m_Position);

    m_View = glm::translate(m_View, glm::vec3(-center.x, -center.y, 0.0f));
    m_NeedToUpdate = false;
}

float Camera::GetRotation() const {
    return m_Rotation;
}

GLIB_NAMESPACE_CLOSE