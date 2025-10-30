#include <thread>

#include "Draw/drawer.h"
#include "FontGenerator/font_generator.h"

static RendererCore::Window window(1000, 1000, "glib");
static GAPI::GraphicsAPIImpl& gapi = GAPI::GraphicsAPIImpl::Get();

int main() {
    GLIB_NAMESPACE_USING;

    using namespace std::chrono_literals;

    Texture tex("resources/images/cat.png");
    Shader customSh("src/lol.glsl");
    customSh.Compile();

    Drawer draw(window);
    Font font("resources/Fonts/Helvetica.ttf");
    auto text = Geom::Text2D("Lor", &font, 12);

    auto mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    mesh.SetScale({100, 100, 1.0f});

    gapi.EnableBlending();
    gapi.BlendFunc(GAPI::BLEND_PARAM::SRC_ALPHA, GAPI::BLEND_PARAM::ONE_MINUS_SRC_ALPHA);

    while (window.IsOpen()) {
        draw.Start();

        draw.DrawText(text, {1.0f, 0.6f, 1.0f, 1.0f}, &customSh);
        draw.DrawMesh(mesh, {1.0f, 1.0f, 1.0f, 1.0f});

        draw.End();
    }

    return 0;
}