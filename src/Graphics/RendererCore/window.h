#pragma once

#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace RendererCore {
    struct Rectangle {
        int32_t x      = 0;
        int32_t y      = 0;
        int32_t width  = 0;
        int32_t height = 0;
    };

    class Window {
    public:
        Window(uint32_t width, uint32_t height, const std::string& name);
        ~Window();

        bool IsOpen();
        void SwapDrawingBuffer();

        int GetWidth() const;
        int GetHeight() const;

        int GetLogicWidth() const;
        int GetLogicHeight() const;

        bool KeyIsPressed(int key);
        void GetCursorPosition(double* w, double* h);

        GLFWwindow* GetWindow() const;

        Rectangle GetViewport() const;
        void ChangeViewport(const Rectangle& rect);
    private:
        Rectangle m_ViewRect;
        GLFWwindow* m_Window = nullptr;
        int m_DiffKof = 0;
    };
}