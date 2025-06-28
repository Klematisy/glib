#include "glib.h"

namespace glib {

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
        UpdateView();
        return m_View;
    }

    void Camera::SetView(const glm::mat4& mat4) {
        m_View = mat4;
    }

    void Camera::SetPosition(const glm::vec2& pos2) {
        m_Position = { pos2, m_Position.z };
    }

    void Camera::SetPosition(const glm::vec3& pos3) {
        m_Position = pos3;
        UpdateView();
    }

    void Camera::SetZoom(float zoom) {
        m_Zoom = zoom;
        UpdateView();
    }

    void Camera::UpdateView() {
        glm::vec2 center((m_Window->GetWidth() / 2), m_Window->GetHeight() / 2);

        m_View = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f));
        m_View = glm::scale(m_View, glm::vec3(m_Zoom, m_Zoom, 1.0f));
        m_View = glm::translate(m_View, glm::vec3(-center, 0.0f));

        m_View = glm::translate(m_View, m_Position);
    }

    const glm::vec3 &Camera::GetPosition() {
        return m_Position;
    }

    float Camera::GetZoom() const {
        return m_Zoom;
    }

}