#include <thread>

#include "Draw/drawer.h"
#include "FontGenerator/font_generator.h"

static RendererCore::Window window(640, 640, "glib");
static GAPI::GraphicsAPIImpl& gapi = GAPI::GraphicsAPIImpl::Get();

int main() {
    GLIB_NAMESPACE_USING;

    using namespace std::chrono_literals;

    Texture tex("resources/images/cat.png");
    Shader customSh("src/lol.glsl");
    customSh.Compile();

    Drawer draw(window);
    Font font("resources/Fonts/Helvetica.ttf");
    auto text = Geom::Text2D("$", &font, 12);

    auto mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    mesh.SetScale({10, 10, 1.0f});

    gapi.EnableBlending();
    gapi.BlendFunc(GAPI::BLEND_PARAM::SRC_ALPHA, GAPI::BLEND_PARAM::ONE_MINUS_SRC_ALPHA);

    while (window.IsOpen()) {
        draw.Start();

        draw.DrawText(text, {1.0f, 1.0f, 1.0f, 1.0f}, &customSh);
//        draw.DrawMesh(mesh, {1.0f, 1.0f, 1.0f, 1.0f});

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
            customSh.HotReload();
            std::this_thread::sleep_for(200ms);
        }

        draw.End();
    }

    return 0;
}