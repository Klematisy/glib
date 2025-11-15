#include "window.h"
#include "renderer.h"
#include "Logger/logger.h"

using namespace RendererCore;
using namespace GAPI;

Window::Window(uint32_t width, uint32_t height, const std::string &name)
    : m_Width(width), m_Height(height)
{
    if (!glfwInit())
        Logger::LogErr("GLFW", "GLFW hasn't initialized!");
    else
        Logger::LogInf("GLFW", "GLFW has initialized!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);

    m_Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (!m_Window) {
        Logger::LogErr("GLFW", "Window hasn't created!");
        glfwTerminate();
    } else {
        Logger::LogInf("GLFW", "Window has created!");
    }

    glfwMakeContextCurrent(m_Window);

    glfwSwapInterval(1);

    static auto& gapi = GraphicsAPIImpl::Get();
    if (gapi.GraphicsInit() != GLEW_OK) {
        Logger::LogErr("OpenGL", "GLEW init error!");
        glfwTerminate();
    } else {
        Logger::LogInf("OpenGL", "GLEW has initialized!");
    }

    using namespace std::string_literals;
    Logger::LogInf("OpenGL", "GL_VERSION: "s + gapi.GetApiVersion());

    glfwGetFramebufferSize(m_Window, &m_RenderWidth, &m_RenderHeight);
}

Window::~Window() {
    glfwTerminate();
}

bool Window::IsOpen() {
    return !glfwWindowShouldClose(m_Window);
}

void Window::SwapDrawingBuffer() {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

int Window::GetWidth() const {
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);
    return windowWidth;
}

int Window::GetHeight() const {
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);
    return windowHeight;
}

GLFWwindow* Window::GetWindow() const {
    return m_Window;
}

int Window::GetRenderFieldWidth() const {
    return m_RenderWidth;
}

int Window::GetRenderFieldHeight() const {
    return m_RenderHeight;
}

void Window::ChangeViewport(int w, int h) {
    m_RenderWidth = w;
    m_RenderHeight = h;

    gapi.Viewport(0, 0, m_RenderWidth, m_RenderHeight);
}