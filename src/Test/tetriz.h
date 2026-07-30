#pragma once

#include <chrono>
#include <thread>

#include "DrawUtils/draw.h"
#include "Geometry/camera.h"
#include "Geometry/mesh.h"
#include "GraphicsAPI/window.h"
#include "GraphicsAPI/graphics_api.h"
#include "FontGenerator/font_generator.h"

using namespace GAPI;
using namespace vladlib;

static bool gameIsRunning = true;

static auto& now = std::chrono::high_resolution_clock::now;
using Time = std::chrono::steady_clock::time_point;
using Duration = std::chrono::duration<float>;
f32 getPassedTime(const Time& tp) {
    Duration dur = now() - tp;
    return dur.count();
}

u0 tetriz() {
    initGraphicsContext(4, 1);
    initGraphicsBackend();

    WindowPTR window = createWindow(600, 600, "TETRIZ");
    SceneRenderer sr;
    sr.Init(window);

    constexpr u32 FPS = 120;
    f32 spf = 0.f;
    Time fps_control_start = now();
    Time fps_encounter_start = now();
    u32 fps_count = 0;

    Font f("resources/fonts/undertale.ttf");
    CharTileInfo tileInfo = f.GetGlyph('u', 30);

    Geom::Entity e;
    e.mesh = std::make_unique<Geom::Mesh>();
    e.transform = std::make_unique<Geom::Transform>();
    e.material = std::make_unique<Geom::Material>();
    *e.mesh = Geom::MeshFactory::Get().CreateMesh("quad");

    e.transform->position = {0, 0, -0.8f};
    e.transform->deltaPivot = {0.5f, 0.5f, 0.0f};

    PerspectiveCamera camera;
    camera.zFar = 1000.f;
    camera.zNear = 0.001f;
    camera.fov = 70.f;
    camera.aspectRatio = 1;
    sr.SetCamera(&camera);

    e.material->image = tileInfo.tex;

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
        sr.DrawEntity(e);
        sr.EndDraw();
    }
}
