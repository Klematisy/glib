#include <iostream>
#include <Draw/draw.h>
#include <Geometry/mesh.h>

static GlCore::Window window(640, 640, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    Draw draw(window);
    Geom::Mesh quad = Geom::MeshFactory::Get().CreateMesh("quad");

    quad.SetScale({50, 50, 1});

    while (window.IsOpen()) {
        draw.Start();

        draw.DrawMesh(quad, {1.0f, 1.0f, 1.0f});

        draw.End();
    }

    return 0;
}