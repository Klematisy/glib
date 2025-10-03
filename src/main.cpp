#include <iostream>
#include <thread>
#include <Draw/drawer.h>
#include <Geometry/mesh.h>

static GlCore::Window window(640, 640, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    using namespace std::chrono_literals;

    Drawer draw(window);

    Font helvetica(LangId::ENG, "resources/Fonts/Helvetica.ttf");
    Geom::Text2D txt("Some text", &helvetica, 3);

    Shader customShader("resources/shaders/user.glsl");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (window.IsOpen()) {
        draw.Start();

        draw.DrawText(txt, {1.0f, 1.0f, 1.0f, 1.0f}, &customShader);

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
            GlCore::ShaderCache::GetCache().HotReload();
            std::this_thread::sleep_for(200ms);
        }

        draw.End();
    }

    return 0;
}