#include "Draw/buffer_drawer.h"
#include "camera.h"

GLIB_NAMESPACE_OPEN

Camera::Camera(const RendererCore::Window* m_Window)
    : m_View(glm::mat4(1.0f)), m_Window(m_Window)
{}

Camera::Camera(glm::vec2 transition2, const RendererCore::Window* m_Window)
    : m_View(glm::mat4(1.0f)), m_Window(m_Window)
{
    m_Position = {transition2, 0.0f};
}

Camera::Camera(glm::vec3 transition3, const RendererCore::Window* m_Window)
    : m_View(glm::mat4(1.0f)), m_Window(m_Window)
{
    m_Position = transition3;
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

void Camera::UpdateView() const {
    glm::vec2 center(m_Window->GetWidth() / 2, m_Window->GetHeight() / 2);

    m_View = glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f));
//    m_View = glm::scale(m_View, glm::vec3(m_Zoom, m_Zoom, 1.0f));
    m_View = glm::translate(m_View, m_Position);
    m_View = glm::rotate(m_View, m_Rotation, glm::vec3(0.0f, 1.0f, 0.0f));

    m_View = glm::translate(m_View, glm::vec3(-center, 0.0f));
    m_NeedToUpdate = false;
}

glm::mat4 Camera::GetVP() const {
    if (m_NeedToUpdate) UpdateView();
    return m_View;
}

float Camera::GetZoom() const     { return m_Zoom;     }
float Camera::GetRotation() const { return m_Rotation; }




PerspectiveCamera::PerspectiveCamera(const RendererCore::Window *m_Window)
    : Camera(m_Window)
{}

glm::mat4 PerspectiveCamera::GetVP() const {
    if (m_NeedToUpdate) UpdateView();
    return glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_ZNear, m_ZFar) * m_View;
}

float PerspectiveCamera::GetAspectRatio() const { return m_AspectRatio; }
float PerspectiveCamera::GetZFar() const        { return m_ZFar;        }
float PerspectiveCamera::GetZNear() const       { return m_ZNear;       }
float PerspectiveCamera::GetFov() const         { return m_Fov;         }

void PerspectiveCamera::SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
void PerspectiveCamera::SetZFar(float zFar)               { m_ZFar = zFar;               }
void PerspectiveCamera::SetZNear(float zNear)             { m_ZNear = zNear;             }
void PerspectiveCamera::SetFov(float fov)                 { m_Fov = fov;                 }




OrthographicCamera::OrthographicCamera(const RendererCore::Window *m_Window)
    : Camera(m_Window)
{}

void OrthographicCamera::SetRenderRange(float left, float right, float bottom, float top, float zNear, float zFar) {
    m_Ortho = glm::ortho(left, right, bottom, top, zNear, zFar);
}

glm::mat4 OrthographicCamera::GetVP() const {
    if (m_NeedToUpdate) UpdateView();
    return m_Ortho * m_View;
}


GLIB_NAMESPACE_CLOSE

