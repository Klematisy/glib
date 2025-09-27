#include <iostream>
#include <thread>
#include <Draw/draw.h>
#include <Geometry/mesh.h>

static GlCore::Window window(640, 640, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    using namespace std::chrono_literals;

    Draw draw(window);
    Shader shader("resources/shaders/user.glsl");

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