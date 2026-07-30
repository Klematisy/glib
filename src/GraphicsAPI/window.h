#pragma once

#include <memory>
#include <string>
#include "common.h"

namespace GAPI {
    enum class KEY : i32 {
        SPACE, APOSTROPHE, COMMA, MINUS, PERIOD, SLASH,
        ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE,
        SEMICOLON, EQUAL, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        LEFT_BRACKET, BACKSLASH, RIGHT_BRACKET, GRAVE_ACCENT, WORLD_1, WORLD_2, ESCAPE, ENTER, TAB, BACKSPACE, INSERT, DELETE,
        RIGHT, LEFT, DOWN, UP, PAGE_UP, PAGE_DOWN, HOME, END, CAPS_LOCK, SCROLL_LOCK, NUM_LOCK, PRINT_SCREEN, PAUSE,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
        KP_0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9, KP_DECIMAL, KP_DIVIDE, KP_MULTIPLY, KP_SUBTRACT, KP_ADD, KP_ENTER, KP_EQUAL,
        LEFT_SHIFT, LEFT_CONTROL, LEFT_ALT, LEFT_SUPER, RIGHT_SHIFT, RIGHT_CONTROL, RIGHT_ALT, RIGHT_SUPER, MENU
    };


    class Window {
    public:
        Window() = default;
        virtual ~Window() = default;

        virtual u0 VSync(bool turn) = 0;

        virtual bool KeyIsPressed(KEY key) = 0;
        virtual bool KeyIsTapped(KEY key) = 0;

        virtual u0 GetCursorPosition(double* w, double* h) = 0;

        virtual Rectanglei GetViewport() const = 0;
        virtual u0 SetViewport(const Rectanglei& rect, int customDiffK = 0) = 0;

        virtual bool IsOpen() = 0;
        virtual u0 SwapDrawingBuffer() = 0;
        virtual u0 UpdateEventPull() = 0;

        virtual i32 GetWidth() const = 0;
        virtual i32 GetHeight() const = 0;

        // virtual i32 GetLogicWidth() const;
        // virtual i32 GetLogicHeight() const;
    };

    using WindowPTR = std::unique_ptr<Window>;

    extern WindowPTR createWindow(u32 w, u32 h, const char* name);
}
