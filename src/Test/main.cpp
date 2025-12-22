#include "Draw/drawer.h"
#include "FontGenerator/font_generator.h"
#include "Utils/atlas.h"

static RendererCore::Window window(600, 600, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    Drawer draw(window);
    Geom::Mesh quad = Geom::MeshFactory::Get().CreateMesh("cube");
    Geom::Mesh screenMesh = Geom::MeshFactory::Get().CreateMesh("quad");

    quad.SetScale({150, 150, 150});
    screenMesh.SetScale({600, 600, 1});
    screenMesh.SetColor({
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f}
    });

    Texture gBlock("resources/images/grass_block.png");
    Texture bmb("resources/images/beautiful_minimalistic_boy.png");



    constexpr float wid = 64;
    constexpr float hei = 48;
    quad.SetUV({
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

    PerspectiveCamera cam(&window);
    OrthographicCamera res_cam(&window);

    glm::vec3 transition(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation(0.0f);

    float spd = 3.0f;
    float rot_spd = 1.0f;

    FrameBaker fb(&window);

    gapi.EnableDepthTest();
    gapi.EnableBlending();
    gapi.BlendFunc(GAPI::BLEND_PARAM::SRC_ALPHA, GAPI::BLEND_PARAM::ONE_MINUS_SRC_ALPHA);

    while (window.IsOpen()) {
        cam.SetFov(70.0f);
        cam.SetAspectRatio(1.0f);
        cam.SetZFar(1000.0f);
        cam.SetZNear(0.1f);

        res_cam.SetRenderRange(0, (float) window.GetWidth(), 0, (float) window.GetHeight(), -1000, 1000);

        if (window.KeyIsPressed(GLFW_KEY_LEFT))  rotation.y -= rot_spd;
        if (window.KeyIsPressed(GLFW_KEY_RIGHT)) rotation.y += rot_spd;
        if (window.KeyIsPressed(GLFW_KEY_UP))    rotation.x -= rot_spd;
        if (window.KeyIsPressed(GLFW_KEY_DOWN))  rotation.x += rot_spd;

        if (window.KeyIsPressed(GLFW_KEY_A)) transition.x -= spd;
        if (window.KeyIsPressed(GLFW_KEY_D)) transition.x += spd;
        if (window.KeyIsPressed(GLFW_KEY_W)) transition.y += spd;
        if (window.KeyIsPressed(GLFW_KEY_S)) transition.y -= spd;
        if (window.KeyIsPressed(GLFW_KEY_I)) transition.z -= spd;
        if (window.KeyIsPressed(GLFW_KEY_K)) transition.z += spd;

        quad.SetPosition(transition);
        quad.SetRotation(rotation);

        draw.Start();

        draw.BeginBake(&fb);
        draw.SetCamera(&cam);
        draw.DrawMesh(quad, &gBlock);
        draw.EndBake();

        draw.SetCamera(&res_cam);
        draw.DrawBakedTexture(screenMesh, fb);

        draw.End();
    }

    return 0;
}
