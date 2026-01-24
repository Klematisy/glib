#include <memory>

#include "DrawUtils/shader.h"
#include "DrawUtils/draw.h"

#include "Geometry/entity.h"
#include "Geometry/camera.h"

#include "Graphics/RendererCore/window.h"

GLIB_NAMESPACE_USING;

int main() {
    RendererCore::Window window(600, 600, "glib");

    RendererCore::ImageInfo bmb("resources/images/beautiful_minimalistic_boy.png");
    RendererCore::ImageInfo cat("resources/images/cat.png");
    RendererCore::ImageInfo wonam("resources/images/wonam.jpg");
    RendererCore::ImageInfo grass_block("resources/images/grass_block.png");
    RendererCore::ImageInfo gayBlock("resources/images/gayBlock.png");

    Geom::Entity e;

    e.mesh = std::make_shared<Geom::Mesh>();
    e.material = std::make_shared<Geom::Material>();
    e.transform = std::make_shared<Geom::Transform>();

    *e.mesh = Geom::MeshFactory::Get().CreateMesh("cube");
    e.material->image = &bmb;
    e.material->uvCoordinates = {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f},
    };

    e.transform->deltaPivot.x = 0.5f;
    e.transform->deltaPivot.y = 0.5f;
    e.transform->deltaPivot.z = 0.5f;
    e.transform->position.x = 1.0f;
    e.transform->position.y = 1.0f;

    OrthographicCamera cam(&window);
    cam.SetRenderRange(0.0f, 2.0f, 0.0f, 2.0f, -100, 100);

    Draw draw(&window);
    draw.UseCamera(&cam);

    gapi.EnableBlending();
    gapi.BlendFunc(GAPI::BLEND_PARAM::SRC_ALPHA, GAPI::BLEND_PARAM::ONE_MINUS_SRC_ALPHA);

    gapi.EnableDepthTest();

    while (window.IsOpen()) {
        draw.StartDraw();

        e.transform->rotation.x += 0.75f;
        e.transform->rotation.y += 0.75f;

        draw.DrawEntity(e);

        draw.EndDraw();
    }

    return 0;
}