#include <array>

#include "Draw/drawer.h"
#include "FontGenerator/font_generator.h"
#include "Utils/atlas.h"

static RendererCore::Window window(600, 600, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    Drawer draw(window);
    Texture tex("resources/images/grass_block.png");

    auto cube = Geom::MeshFactory::Get().CreateMesh("cube");

    constexpr float wid = 64;
    constexpr float hei = 48;
    cube.SetUV({
        0  / wid, 16 / hei,
        16 / wid, 16 / hei,
        16 / wid, 32 / hei,
        0  / wid, 32 / hei,

        0  / wid, 16 / hei,
        16 / wid, 16 / hei,
        16 / wid, 32 / hei,
        0  / wid, 32 / hei,

        0  / wid, 16 / hei,
        16 / wid, 16 / hei,
        16 / wid, 32 / hei,
        0  / wid, 32 / hei,

        0  / wid, 16 / hei,
        16 / wid, 16 / hei,
        16 / wid, 32 / hei,
        0  / wid, 32 / hei,

        16 / wid, 0  / hei,
        32 / wid, 0  / hei,
        32 / wid, 16 / hei,
        16 / wid, 16 / hei,

        16 / wid, 32 / hei,
        32 / wid, 32 / hei,
        32 / wid, 48 / hei,
        16 / wid, 48 / hei,
    });

    cube.SetScale({200, 200, 200});

    Camera cam(&window);
    glm::vec3 transition(0.0f);
    glm::vec3 rotation(0.0f);

    draw.SetCamera(&cam);

    float spd = 2.0f;
    float rot_spd = 0.8f;

    glEnable(GL_DEPTH_TEST);

    while (window.IsOpen()) {
        if (window.KeyIsPressed(GLFW_KEY_UP))    rotation.x += rot_spd;
        if (window.KeyIsPressed(GLFW_KEY_DOWN))  rotation.x -= rot_spd;
        if (window.KeyIsPressed(GLFW_KEY_RIGHT)) rotation.y += rot_spd;
        if (window.KeyIsPressed(GLFW_KEY_LEFT))  rotation.y -= rot_spd;

        if (window.KeyIsPressed(GLFW_KEY_W)) transition.y -= spd;
        if (window.KeyIsPressed(GLFW_KEY_S)) transition.y += spd;
        if (window.KeyIsPressed(GLFW_KEY_A)) transition.x -= spd;
        if (window.KeyIsPressed(GLFW_KEY_D)) transition.x += spd;

        cube.SetRotation(rotation);
        cube.SetPosition(transition);

        draw.Start();

        draw.DrawMesh(cube, &tex);

        draw.End();
    }

    return 0;
}
