#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "../common.h"
#include "glfw_window.h"
#include "GraphicsAPI/window.h"
#include "Logger/logger.h"


static const i32 KEY_ARRAY[] {
    GLFW_KEY_SPACE, GLFW_KEY_APOSTROPHE, GLFW_KEY_COMMA, GLFW_KEY_MINUS, GLFW_KEY_PERIOD, GLFW_KEY_SLASH,
    GLFW_KEY_0, GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9,
    GLFW_KEY_SEMICOLON, GLFW_KEY_EQUAL, GLFW_KEY_A, GLFW_KEY_B, GLFW_KEY_C, GLFW_KEY_D, GLFW_KEY_E, GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H, GLFW_KEY_I, GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_M, GLFW_KEY_N, GLFW_KEY_O, GLFW_KEY_P, GLFW_KEY_Q, GLFW_KEY_R, GLFW_KEY_S, GLFW_KEY_T, GLFW_KEY_U, GLFW_KEY_V, GLFW_KEY_W, GLFW_KEY_X, GLFW_KEY_Y, GLFW_KEY_Z,
    GLFW_KEY_LEFT_BRACKET, GLFW_KEY_BACKSLASH, GLFW_KEY_RIGHT_BRACKET, GLFW_KEY_GRAVE_ACCENT, GLFW_KEY_WORLD_1, GLFW_KEY_WORLD_2, GLFW_KEY_ESCAPE, GLFW_KEY_ENTER, GLFW_KEY_TAB, GLFW_KEY_BACKSPACE, GLFW_KEY_INSERT, GLFW_KEY_DELETE,
    GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_PAGE_UP, GLFW_KEY_PAGE_DOWN, GLFW_KEY_HOME, GLFW_KEY_END, GLFW_KEY_CAPS_LOCK, GLFW_KEY_SCROLL_LOCK, GLFW_KEY_NUM_LOCK, GLFW_KEY_PRINT_SCREEN, GLFW_KEY_PAUSE,
    GLFW_KEY_F1, GLFW_KEY_F2, GLFW_KEY_F3, GLFW_KEY_F4, GLFW_KEY_F5, GLFW_KEY_F6, GLFW_KEY_F7, GLFW_KEY_F8, GLFW_KEY_F9, GLFW_KEY_F10, GLFW_KEY_F11, GLFW_KEY_F12, GLFW_KEY_F13, GLFW_KEY_F14, GLFW_KEY_F15, GLFW_KEY_F16, GLFW_KEY_F17, GLFW_KEY_F18, GLFW_KEY_F19, GLFW_KEY_F20, GLFW_KEY_F21, GLFW_KEY_F22, GLFW_KEY_F23, GLFW_KEY_F24, GLFW_KEY_F25,
    GLFW_KEY_KP_0, GLFW_KEY_KP_1, GLFW_KEY_KP_2, GLFW_KEY_KP_3, GLFW_KEY_KP_4, GLFW_KEY_KP_5, GLFW_KEY_KP_6, GLFW_KEY_KP_7, GLFW_KEY_KP_8, GLFW_KEY_KP_9, GLFW_KEY_KP_DECIMAL, GLFW_KEY_KP_DIVIDE, GLFW_KEY_KP_MULTIPLY, GLFW_KEY_KP_SUBTRACT, GLFW_KEY_KP_ADD, GLFW_KEY_KP_ENTER, GLFW_KEY_KP_EQUAL,
    GLFW_KEY_LEFT_SHIFT, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_LEFT_ALT, GLFW_KEY_LEFT_SUPER, GLFW_KEY_RIGHT_SHIFT, GLFW_KEY_RIGHT_CONTROL, GLFW_KEY_RIGHT_ALT, GLFW_KEY_RIGHT_SUPER, GLFW_KEY_MENU
};


namespace GAPI {
    void initGraphicsContext(u32 majorV, u32 minorV) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorV);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorV);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    }
}

using namespace GAPI;

WindowGLFW::WindowGLFW()
{
    if (!glfwInit())
        LOGINF("GLFW: GLFW hasn't initialized!");
    else
        LOGINF("GLFW: GLFW has initialized!");
}

void WindowGLFW::CreateWindow(uint32_t width, uint32_t height, const std::string& name) {
    m_Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (!m_Window) {
        LOGERR("GLFW: Window hasn't created!");
        glfwTerminate();
    } else {
        LOGINF("GLFW: Window has created!");
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(0);

    glfwGetFramebufferSize(m_Window, &m_ViewRect.width, &m_ViewRect.height);

    int lh = GetLogicHeight();
    int h = GetHeight();
    m_DiffKof = lh / h;
}

WindowGLFW::~WindowGLFW() {
    glfwTerminate();
}

bool WindowGLFW::IsOpen() {
    return !glfwWindowShouldClose(m_Window);
}

void WindowGLFW::SwapDrawingBuffer() {
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

int WindowGLFW::GetWidth() const {
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);
    return windowWidth;
}

int WindowGLFW::GetHeight() const {
    int windowWidth, windowHeight;
    glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);
    return windowHeight;
}

Rectanglei WindowGLFW::GetViewport() const {
    Rectanglei rect = m_ViewRect;
    rect.x /= m_DiffKof;
    rect.y /= m_DiffKof;
    rect.width /= m_DiffKof;
    rect.height /= m_DiffKof;

    return rect;
}

void WindowGLFW::SetViewport(const Rectanglei& rect, int customDiffK) {
    int height = GetHeight();

    if (customDiffK == 0) customDiffK = m_DiffKof;

    m_ViewRect = {rect.x * customDiffK,
                  rect.y * customDiffK,
                  rect.width * customDiffK,
                  rect.height * customDiffK};

    glViewport(m_ViewRect.x, m_ViewRect.y, m_ViewRect.width, m_ViewRect.height);
}

int WindowGLFW::GetLogicWidth() const {
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(m_Window, &windowWidth, &windowHeight);
    return windowWidth;
}

int WindowGLFW::GetLogicHeight() const {
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(m_Window, &windowWidth, &windowHeight);
    return windowHeight;
}

void WindowGLFW::VSync(bool turn) {
    glfwSwapInterval(turn);
}

bool WindowGLFW::KeyIsPressed(GAPI::KEY key) {
    return glfwGetKey(m_Window, KEY_ARRAY[static_cast<int>(key)]) == GLFW_PRESS;
}

bool WindowGLFW::KeyIsTapped(GAPI::KEY key) {
    bool result = glfwGetKey(m_Window, KEY_ARRAY[static_cast<int>(key)]) == GLFW_PRESS;
    if (result == !m_KeyTapped && result) {
        m_KeyTapped = true;
    } else if (result == !m_KeyTapped && !result) {
        m_KeyTapped = false;
    } else {
        result = false;
    }

    return result;
}

void WindowGLFW::GetCursorPosition(double* w, double* h) {
    glfwGetCursorPos(m_Window, w, h);
}
