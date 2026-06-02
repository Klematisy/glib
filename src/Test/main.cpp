#include "Graphics/RendererCore/renderer.h"
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

void CompareFrameBakerWithWindow(vladlib::FrameBaker& fm, const RendererCore::Window& w) {
    fm.image = RendererCore::ImageInfo(w.GetWidth(), w.GetHeight(), fm.image.GetBPP(), nullptr);
}

int main() {
    using namespace vladlib;
    namespace rc = RendererCore;

    rc::Window window({4, 1});
    window.CreateWindow(600, 600, "vlad");
    rc::rendererAPI->GraphicsInit();

    using namespace std::string_literals;
    LOGINF("OpenGL ver: "s + rc::rendererAPI->GetApiVersion());
    SceneRenderer sr(&window);

    rc::rendererAPI->EnableDepthTest();

    rc::ImageInfo image1("resources/images/atlas.png");
    rc::ImageInfo image2("resources/images/hud.png");

    Geom::Entity quad1, quad2, quad3;
    quad1.transform = std::make_unique<Geom::Transform>();
    quad1.material = std::make_unique<Geom::Material>();
    quad1.mesh = std::make_unique<Geom::Mesh>();
    *quad1.mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    quad1.transform->deltaPivot = {0.5f, 0.5f, 0.5f};

    quad2 = quad1;
    quad3 = quad1;

    quad1.transform->position = {0.5f, 0.5f, 0.0f};
    quad2.transform->position = {0.5f, 0.5f, 0.1f};
    quad3.transform->position = {0.5f, 0.5f, 0.2f};

    // quad2.material->uvCoordinates = {
    //     {0.f, 2.f},
    //     {0.f, 0.f},
    //     {2.f, 0.f},
    //     {2.f, 2.f},
    // };

    quad1.material->image = &image1;
    quad2.material->image = &image2;

    OrthographicCamera camera;
    camera.left = 0;
    camera.right = 1;
    camera.bottom = 0;
    camera.top = 1;

    sr.UseCamera(&camera);

    do {
        sr.StartDraw();

        sr.DrawEntity(quad1);
        sr.DrawEntity(quad3);
        // sr.DrawEntity(quad2);

        sr.EndDraw();
    // } while (0);
    } while (window.IsOpen());

    return 0;
}

#endif
