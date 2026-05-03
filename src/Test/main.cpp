#if 1
#include "vladoom.h"

int main() {
    vladoom();
}

#else

#include "DrawUtils/draw.h"
#include "Geometry/entity.h"
#include "Graphics/RendererCore/window.h"

auto& now = std::chrono::high_resolution_clock::now;
using Time = std::chrono::steady_clock::time_point;
float getPassedTime(const Time& tp) {
    std::chrono::duration<float> dur = now() - tp;
    return dur.count();
}

int main() {
    using namespace vladlib;
    namespace rc = RendererCore;

    rc::Window window(600, 600, "vlad");
    SceneRenderer sr(&window);

    rc::ImageInfo image("resources/images/beautiful_minimalistic_boy.png");
    image.SetTexParam({
        .magFilter = GAPI::TEXTURE_PARAM::LINEAR,
        .minFilter = GAPI::TEXTURE_PARAM::LINEAR
    });

    Geom::Entity quad, screen;
    quad.transform = std::make_unique<Geom::Transform>();
    quad.material = std::make_unique<Geom::Material>();
    quad.mesh = std::make_unique<Geom::Mesh>();
    *quad.mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    quad.transform->deltaPivot = {0.5f, 0.5f, 0.5f};
    quad.transform->position = {1.f, 1.f, 0.f};
    quad.material->image = &image;

    screen = quad;
    screen.transform->scale = {2.f, 2.f, 1.f};

    OrthographicCamera camera;
    camera.SetRenderRange(0, 2, 0, 2);
    sr.UseCamera(&camera);

    FrameBaker fb;

    Shader shader;
    shader.AddSrcFiles("src/Test/lol.glsl");
    shader.Compile();

    screen.material->shader = &shader;
    screen.material->image = &fb.GetImage();
    fb.GetImage().SetTexParam({
        .magFilter = GAPI::TEXTURE_PARAM::LINEAR,
        .minFilter = GAPI::TEXTURE_PARAM::LINEAR
    });

    sr.RegisterFrameBaker(fb);

    float a = 0.f;

    constexpr float FPS = 60.f;

    Time fps_control_start = now();
    while (window.IsOpen()) {
        if (getPassedTime(fps_control_start) < (1.f / FPS)) continue;
        else fps_control_start = now();

        sr.StartDraw();
        a += 1.f;

        sr.StartBake(fb);
        sr.DrawEntity(quad);
        sr.EndBake();

        if (window.KeyIsTapped(GLFW_KEY_R)) {
            shader.HotReload();

            LOGINF("HOT RELOAD");
        }

        shader.GetShaderProgram()->SetFloat("u_Time", a);
        sr.DrawEntity(screen);

        sr.EndDraw();
    }

    return 0;
}

#endif