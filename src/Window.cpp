#include "Window.h"
#include "OpenGLCore/Renderer.h"
#include "OpenGLCore/Logger.h"

GlCore::Window::Window(uint32_t width, uint32_t height, const std::string &name)
    : m_Width(width), m_Height(height)
{
    if (!glfwInit())
        Logger::Logln(Logger::LogLevel::ERROR, "GLFW hasn't initialized!");
    else
        Logger::Logln(Logger::LogLevel::INFO, "GLFW has initialized!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (!m_Window) {
        Logger::Logln(Logger::LogLevel::ERROR, "Window hasn't created!");
        glfwTerminate();
    } else {
        Logger::Logln(Logger::LogLevel::INFO, "Window has created!");
    }

    glfwMakeContextCurrent(m_Window);

    if (glewInit() != GLEW_OK) {
        Logger::Logln(Logger::LogLevel::ERROR, "OpenGL: GLEW init error!");
        glfwTerminate();
    } else {
        Logger::Logln(Logger::LogLevel::INFO, "OpenGL: GLEW has initialized!");
    }

    using namespace std::string_literals;
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    Logger::Log(Logger::LogLevel::INFO, "OpenGL: VERSION: "s + version);

    GlCore::ShaderCache::GetCache().LoadCache();
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
