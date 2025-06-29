#include "Window.h"
#include "OpenGLCore/Renderer.h"

GlCore::Window::Window(uint32_t width, uint32_t height, const std::string &name)
    : m_Width(width), m_Height(height)
{
    if (!glfwInit())
        std::cerr << "GLFW hasn't initialized!" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        std::cerr << "Window hasn't created!" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(m_Window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Glew init error!\n" << std::endl;
        glfwTerminate();
    }

    GlCore::Cache.LoadCache();
}

GlCore::Window::~Window() {
    glfwTerminate();
}

bool GlCore::Window::IsOpen() {
    return !glfwWindowShouldClose(m_Window);
}

void GlCore::Window::SwapDrawingBuffer() {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

uint32_t GlCore::Window::GetWidth() const {
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);
    return windowWidth;
}

uint32_t GlCore::Window::GetHeight() const {
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);
    return windowHeight;
}

GLFWwindow* GlCore::Window::GetWindow() const {
    return m_Window;
}
