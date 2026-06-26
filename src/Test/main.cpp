#include <chrono>
#if 0
#include "vladoom.h"

int main() {
    vladoom();
}

#else

#include <thread>

#include "DrawUtils/draw.h"
#include "Geometry/entity.h"
#include "GraphicsAPI/graphics_api.h"
#include "GraphicsAPI/window.h"

auto& now = std::chrono::high_resolution_clock::now;
using Time = std::chrono::steady_clock::time_point;
using Duration = std::chrono::duration<float>;
float getPassedTime(const Time& tp) {
    Duration dur = now() - tp;
    return dur.count();
}

void CompareFrameBakerWithWindow(vladlib::FrameBaker& fm, const GAPI::Window& w) {
    fm.image = GAPI::ImageInfo(w.GetWidth(), w.GetHeight(), fm.image.GetBPP(), nullptr);
}

int main() {
    using namespace vladlib;

    std::shared_ptr<GAPI::Window> window = GAPI::createWindow();
    GAPI::initGraphicsContext(4, 1);
    window->CreateWindow(600, 600, "vlad");
    GAPI::initGraphicsBackend();

    using namespace std::string_literals;
    LOGINF("OpenGL ver: "s + GAPI::getApiVersion());
    SceneRenderer sr(window.get());

    GAPI::enableDepthTest();

    Shader myCustomShader;
    myCustomShader.AddSrcFile("resources/shaders/main.glsl", GAPI::SHADER_TYPE::VERTEX | GAPI::SHADER_TYPE::FRAGMENT);
    myCustomShader.Compile();

    Geom::Entity quad1;
    quad1.transform = std::make_unique<Geom::Transform>();
    quad1.material = std::make_unique<Geom::Material>();
    quad1.mesh = std::make_unique<Geom::Mesh>();
    *quad1.mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    quad1.transform->deltaPivot = {0.5f, 0.5f, 0.5f};
    quad1.transform->position = {0.5f, 0.5f, 0.0f};
    quad1.material->shader = &myCustomShader;

    OrthographicCamera camera;
    camera.left = 0;
    camera.right = 1;
    camera.top = 0;
    camera.bottom = 1;

    sr.UseCamera(&camera);

    constexpr uint32_t FPS = 120;
    float spf = 0.f;
    Time fps_control_start = now();
    Time fps_encounter_start = now();
    uint32_t fps_count = 0;

    float phi = 0.f;
    float spd = 5.f;

    auto a = now();
    while (window->IsOpen()) {
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
            break;
        }

        if (window->KeyIsTapped(GAPI::KEY::R)) {
            myCustomShader.HotReload();
        }

        if (window->KeyIsPressed(GAPI::KEY::LEFT)) {
            phi -= spd * spf;
        }

        if (window->KeyIsPressed(GAPI::KEY::RIGHT)) {
            phi += spd * spf;
        }

        sr.StartDraw();

        myCustomShader.GetShaderProgram()->SetFloat2("u_Resolution", {window->GetWidth(), window->GetHeight()});
        myCustomShader.GetShaderProgram()->SetFloat("u_Time", getPassedTime(a));
        sr.DrawEntity(quad1);

        sr.EndDraw();
    }

    return 0;
}

#endif
