#pragma once

#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace RendererCore {
    struct Rectangle {
        int x      = 0;
        int y      = 0;
        int width  = 0;
        int height = 0;
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

        GLFWwindow* GetWindow() const;

        Rectangle GetViewport() const;
        void ChangeViewport(const Rectangle& rect);
    private:
        Rectangle m_ViewRect;
        GLFWwindow* m_Window = nullptr;
        int m_DiffKof = 0;
    };
}