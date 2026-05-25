#pragma once

#include "GLFW/glfw3.h"

namespace RendererCore {
    struct Rectanglei {
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

        void VSync(bool turn);

        bool KeyIsPressed(int key);
        bool KeyIsTapped(int key);

        void GetCursorPosition(double* w, double* h);

        GLFWwindow* GetWindow() const;

        Rectanglei GetViewport() const;
        void ChangeViewport(const Rectanglei& rect, int customDiffK = 0);
    private:
        bool m_KeyTapped = false;

        Rectanglei m_ViewRect;
        GLFWwindow* m_Window = nullptr;
        int m_DiffKof = 0;
    };
}
