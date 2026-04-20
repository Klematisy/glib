#if 1
#include "vladoom.h"

int main() {
    vladoom();
}

#else

#include "DrawUtils/draw.h"
#include "Geometry/entity.h"
#include "Graphics/RendererCore/window.h"

int main() {
    using namespace RendererCore;
    using namespace glib;

    Window window(600, 600, "vlad");
    glib::Draw draw(&window);

    Geom::Entity quad;
    quad.transform = std::make_shared<Geom::Transform>();
    quad.material = std::make_shared<Geom::Material>();
    quad.mesh = std::make_shared<Geom::Mesh>();
    *quad.mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    quad.transform->deltaPivot = {0.5f, 0.5f, 0.5f};
    quad.transform->position = {1.f, 1.f, 0.f};

    OrthographicCamera camera;
    camera.SetRenderRange(0, 2, 0, 2);
    draw.UseCamera(&camera);

    FrameBaker fb;

    while (window.IsOpen()) {
        draw.StartDraw();

        draw.DrawEntity(quad);

        draw.EndDraw();
    }

    return 0;
}

#endif