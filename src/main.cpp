#include "Draw/drawer.h"
#include "Geometry/mesh.h"
#include "FontGenerator/font_generator.h"

static GlCore::Window window(640, 640, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    using namespace std::chrono_literals;
    Drawer draw(window);

    Texture tex("resources/images/cat.png");
    Shader myShader;
    myShader.AddSrcFiles("src/user.glsl");
    myShader.Compile();

    auto mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    mesh.SetScale({tex.GetWidth() / 2, tex.GetHeight() / 2, 1});
    mesh.SetPosition({0.0f, 0.0f, 1.0f});

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (window.IsOpen()) {
        draw.Start();

        draw.DrawMesh(mesh, {0.1f, 0.5f, 0.7f, 1.0f});
        draw.DrawMesh(mesh, {1.0f, 1.0f, 1.0f, 1.0f}, &tex, &myShader);

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
            myShader.HotReload();
        }

        draw.End();
    }

    return 0;
}