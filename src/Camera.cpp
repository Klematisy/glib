#include "glib.h"

namespace glib {

    Camera::Camera()
        : m_View(glm::mat4(1.0f))
    {}

    Camera::Camera(glm::vec2 transition2)
        : m_View(transition(transition2))
    {}

    Camera::Camera(glm::vec3 transition3)
        : m_View(transition(transition3))
    {}

    const glm::mat4 &Camera::GetView() {
        return m_View;
    }

    void Camera::SetView(const glm::mat4& mat4) {
        m_View = mat4;
    }

    const glm::mat4 &Camera::transition(const glm::vec2& transition2) {
        glm::vec3 transition3(transition2.x, transition2.y, 0.0f);
        m_View = glm::translate(m_View, transition3);
        return m_View;
    }

    const glm::mat4 &Camera::transition(const glm::vec3& transition3) {
        m_View = glm::translate(m_View, transition3);
        return m_View;
    }

}