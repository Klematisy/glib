#include "renderer.h"
#include "window.h"
#include "Logger/logger.h"

using namespace RendererCore;
using namespace GAPI;
static const auto gapi = rendererAPI;


Window::Window(RendererContext context)
{
    if (!glfwInit())
        LOGINF("GLFW: GLFW hasn't initialized!");
    else
        LOGINF("GLFW: GLFW has initialized!");

    rendererContext->Init();
    rendererContext->SetMajorV(context.majVer);
    rendererContext->SetMinorV(context.minVer);
}

void Window::CreateWindow(uint32_t width, uint32_t height, const std::string& name) {
    m_Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (!m_Window) {
        LOGERR("GLFW: Window hasn't created!");
        glfwTerminate();
    } else {
        LOGINF("GLFW: Window has created!");
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(0);

    stbi_set_flip_vertically_on_load(1);

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

Rectanglei Window::GetViewport() const {
    Rectanglei rect = m_ViewRect;
    rect.x /= m_DiffKof;
    rect.y /= m_DiffKof;
    rect.width /= m_DiffKof;
    rect.height /= m_DiffKof;

    return rect;
}

void Window::ChangeViewport(const Rectanglei& rect, int customDiffK) {
    int height = GetHeight();

    if (customDiffK == 0) customDiffK = m_DiffKof;

    m_ViewRect = {rect.x * customDiffK,
                  rect.y * customDiffK,
                  rect.width * customDiffK,
                  rect.height * customDiffK};

    gapi->Viewport(m_ViewRect.x, m_ViewRect.y, m_ViewRect.width, m_ViewRect.height);
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

void Window::VSync(bool turn) {
    glfwSwapInterval(turn);
}

bool Window::KeyIsPressed(int key) {
    return glfwGetKey(m_Window, key) == GLFW_PRESS;
}

bool Window::KeyIsTapped(int key) {
    bool result = glfwGetKey(m_Window, key) == GLFW_PRESS;
    if (result == !m_KeyTapped && result) {
        m_KeyTapped = true;
    } else if (result == !m_KeyTapped && !result) {
        m_KeyTapped = false;
    } else {
        result = false;
    }

    return result;
}

void Window::GetCursorPosition(double* w, double* h) {
    glfwGetCursorPos(m_Window, w, h);
}
