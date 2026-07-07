#pragma once

#include "../window.h"
#include "GLFW/glfw3.h"

class WindowGLFW : public GAPI::Window {
public:
    WindowGLFW(u32 width, u32 height, const std::string& name);
    ~WindowGLFW();

    u0 VSync(bool turn) override;

    bool KeyIsPressed(GAPI::KEY key) override;
    bool KeyIsTapped(GAPI::KEY key) override;

    u0 GetCursorPosition(double* w, double* h) override;

    Rectanglei GetViewport() const override;
    u0 SetViewport(const Rectanglei& rect, int customDiffK = 0) override;

    bool IsOpen() override;
    u0 SwapDrawingBuffer() override;
    u0 UpdateEventPull() override;

    i32 GetWidth() const override;
    i32 GetHeight() const override;

    i32 GetLogicWidth() const;
    i32 GetLogicHeight() const;
private:
    GLFWwindow* m_Window = nullptr;
    Rectanglei m_ViewRect;

    bool m_KeyTapped = false;
    int m_DiffKof = 0;
};
