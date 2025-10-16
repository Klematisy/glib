#include "Draw/drawer.h"
#include "Geometry/mesh.h"
#include "FontGenerator/font_generator.h"
#include "Draw/shader.h"

static GlCore::Window window(640, 640, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    using namespace std::chrono_literals;
    Drawer draw(window);

    Texture tex("resources/images/cat.png");
    Shader myShader;
    myShader.AddSrcFiles({"resources/shaders/user.glsl"});
    myShader.Compile();

    auto mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    mesh.SetScale({tex.GetWidth() / 2, tex.GetHeight() / 2, 1});
    mesh.SetPosition({0.0f, 0.0f, 1.0f});

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (window.IsOpen()) {
        draw.Start();

        draw.DrawMesh(mesh, {1.0f, 1.0f, 1.0f, 1.0f}, &tex, &myShader);

        draw.End();
    }

    return 0;
}