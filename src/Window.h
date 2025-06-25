#pragma once

#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace GlCore {

    class Window {
    public:
        Window(uint32_t width, uint32_t height, const std::string &name);

        bool IsOpen();

        void SwapDrawingBuffer();

        uint32_t GetWidth() const;

        uint32_t GetHeight() const;

        GLFWwindow *GetWindow() const;

        ~Window();

    private:
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        GLFWwindow *m_Window = nullptr;
    };

}