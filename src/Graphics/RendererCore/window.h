#pragma once

#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace RendererCore {
    class Window {
    public:
        Window(uint32_t width, uint32_t height, const std::string& name);
        ~Window();

        bool IsOpen();
        void SwapDrawingBuffer();

        int GetWidth() const;
        int GetHeight() const;
        GLFWwindow* GetWindow() const;

        int GetRenderFieldWidth() const;
        int GetRenderFieldHeight() const;

        void ChangeViewport(int w, int h);
    private:
        int m_RenderWidth = 0;
        int m_RenderHeight = 0;

        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        GLFWwindow* m_Window = nullptr;
    };
}