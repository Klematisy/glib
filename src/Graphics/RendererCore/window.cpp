#include "window.h"
#include "renderer.h"
#include "Logger/logger.h"

using namespace RendererCore;
using namespace GAPI;

Window::Window(uint32_t width, uint32_t height, const std::string& name)
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

    glfwGetFramebufferSize(m_Window, &m_ViewRect.width, &m_ViewRect.height);

    int lh = GetLogicHeight();
    int h = GetHeight();
    m_DiffKof = lh / h;
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

Rectangle Window::GetViewport() const {
    Rectangle rect = m_ViewRect;
    rect.x /= m_DiffKof;
    rect.y /= m_DiffKof;
    rect.width /= m_DiffKof;
    rect.height /= m_DiffKof;

    return rect;
}

void Window::ChangeViewport(const Rectangle& rect) {
    int height = GetHeight();

    m_ViewRect.x = (int) (rect.x * m_DiffKof);
    m_ViewRect.y = (int) ((float) (height - rect.height - rect.y) * (float) m_DiffKof);
    m_ViewRect.width = (int) (rect.width * m_DiffKof);
    m_ViewRect.height = (int) (rect.height * m_DiffKof);

    gapi.Viewport(m_ViewRect.x, m_ViewRect.y, m_ViewRect.width, m_ViewRect.height);
}

int Window::GetLogicWidth() const {
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(m_Window, &windowWidth, &windowHeight);
    return windowWidth;
}

int Window::GetLogicHeight() const {
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(m_Window, &windowWidth, &windowHeight);
    return windowHeight;
}

bool Window::KeyIsPressed(int key) {
    return glfwGetKey(m_Window, key) == GLFW_PRESS;
}

void Window::GetCursorPosition(double* w, double* h) {
    glfwGetCursorPos(m_Window, w, h);
}
