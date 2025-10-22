#include <thread>

#include "Draw/drawer.h"
#include "FontGenerator/font_generator.h"

static GlCore::Window window(640, 640, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    using namespace std::chrono_literals;

    Texture tex("resources/images/cat.png");
    Shader customSh("resources/shaders/font.glsl");
    customSh.Compile();

    Drawer draw(window);
    Font font("resources/Fonts/Helvetica.ttf");
    auto text = Geom::Text2D("A", &font, 40);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_FRAMEBUFFER_SRGB);

    while (window.IsOpen()) {
        draw.Start();

        draw.DrawText(text, {1.0f, 1.0f, 1.0f, 1.0f}, &customSh);

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
            customSh.HotReload();
            std::this_thread::sleep_for(200ms);
        }

        draw.End();
    }

    return 0;
}