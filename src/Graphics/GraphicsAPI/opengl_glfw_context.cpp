#include <cinttypes>
#include "opengl_glfw_context.h"
#include "GLFW/glfw3.h"

GAPI_NAMESPACE_USING;

OpenGLGLFWContext& OpenGLGLFWContext::Get() {
    static OpenGLGLFWContext context;
    return context;
}

void OpenGLGLFWContext::Init(uint32_t major, uint32_t minor) {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    m_MajorV = major;
    m_MinorV = minor;
}

uint32_t OpenGLGLFWContext::GetMajorV() {
    return m_MajorV;
}

uint32_t OpenGLGLFWContext::GetMinorV() {
    return m_MinorV;
}
