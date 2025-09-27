#include <iostream>
#include <thread>
#include <Draw/drawer.h>
#include <Geometry/mesh.h>

static GlCore::Window window(640, 640, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    using namespace std::chrono_literals;

    Drawer draw(window);

    // Geom::Text txt("Some text", 200);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (window.IsOpen()) {
        draw.Start();

        // draw.DrawText(txt, &shader);

        draw.End();
    }

    return 0;
}