#include "camera.h"

VLADLIB_NAMESPACE_OPEN

glm::mat4 Camera::CalculateView() const {
    glm::mat4 view(1.0f);

    view = glm::rotate(view, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::rotate(view, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    view = glm::translate(view, -transform.position);

    return view;
}

glm::mat4 Camera::GetVP() const { return CalculateView(); }
glm::mat4 Camera::GetView() const { return CalculateView(); }
glm::mat4 Camera::GetProject() const { return glm::mat4(1.0f); }



PerspectiveCamera::PerspectiveCamera()
    : Camera()
{
    zNear = 0.01f;
    zFar = 1.f;
}

glm::mat4 PerspectiveCamera::GetVP() const {
    return glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar) * CalculateView();
}

glm::mat4 PerspectiveCamera::GetProject() const {
    return glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
}



OrthographicCamera::OrthographicCamera()
    : Camera()
{
    zNear = -1.f;
    zFar = 1.f;
}

glm::mat4 OrthographicCamera::GetVP() const {
    return glm::ortho(left, right, bottom, top, zNear, zFar) * CalculateView();
}

glm::mat4 OrthographicCamera::GetProject() const {
    return glm::ortho(left, right, bottom, top, zNear, zFar);
}


VLADLIB_NAMESPACE_CLOSE

