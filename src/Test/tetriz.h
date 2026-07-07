#pragma once

#include <chrono>
#include <thread>

#include "DrawUtils/draw.h"
#include "GraphicsAPI/window.h"
#include "GraphicsAPI/graphics_api.h"

using namespace GAPI;
using namespace vladlib;

static bool gameIsRunning = true;

static auto& now = std::chrono::high_resolution_clock::now;
using Time = std::chrono::steady_clock::time_point;
using Duration = std::chrono::duration<float>;
float getPassedTime(const Time& tp) {
    Duration dur = now() - tp;
    return dur.count();
}

void tetriz() {
    initGraphicsContext(4, 1);
    initGraphicsBackend();

    WindowPTR window = createWindow(600, 600, "TETRIZ");
    SceneRenderer sr(window);

    constexpr uint32_t FPS = 120;
    float spf = 0.f;
    Time fps_control_start = now();
    Time fps_encounter_start = now();
    uint32_t fps_count = 0;

    while (gameIsRunning && window->IsOpen()) {
        float passedTime = getPassedTime(fps_control_start);
        Duration sleepTime = Duration(1.f / FPS - passedTime) - std::chrono::milliseconds(1);
        if (sleepTime.count() > 0.f) {
            std::this_thread::sleep_for(sleepTime);
        }

        passedTime = getPassedTime(fps_control_start);
        while (getPassedTime(fps_control_start) < 1.f / FPS)
        {
            std::this_thread::yield();
        }
        fps_control_start = now();

        fps_count++;
        if (getPassedTime(fps_encounter_start) > 1.f) {
            fps_encounter_start = now();
            spf = 1.f / (float)fps_count;
            fps_count = 0;
        }

        if (window->KeyIsPressed(GAPI::KEY::ESCAPE)) {
            gameIsRunning = false;
        }

        sr.StartDraw();
        sr.EndDraw();
    }
}
