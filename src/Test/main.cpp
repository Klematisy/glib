#include "Draw/drawer.h"
#include "FontGenerator/font_generator.h"
#include "Utils/atlas.h"

static RendererCore::Window window(600, 600, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    Drawer draw(window);
    Font font("resources/Fonts/Helvetica.ttf");
    Geom::Text2D text2D("Lol_IT_abc", &font, 4);
    text2D.ReadMesh()->SetColor({
        {1.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 1.0f, 1.0f}
    });

    Geom::Mesh quad = Geom::MeshFactory::Get().CreateMesh("quad");
    Geom::Mesh screenMesh = Geom::MeshFactory::Get().CreateMesh("quad");

    quad.SetScale({150, 150, 1});
    screenMesh.SetScale({600, 600, 1});
    screenMesh.SetColor({
            {0.0f, 1.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 0.0f, 1.0f}
    });

    Texture tex("resources/images/grass_block.png");
    Texture bmb("resources/images/beautiful_minimalistic_boy.png");

    auto cube = Geom::MeshFactory::Get().CreateMesh("cube");

    OrthographicCamera cam(&window);
    OrthographicCamera res_cam(&window);

    cam.SetRenderRange(0, (float) window.GetWidth(), (float) window.GetHeight(), 0, -1000, 1000);
    draw.SetCamera(&cam);

    glm::vec3 transition(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation(0.0f);

    float spd = 3.0f;

//    glEnable(GL_DEPTH_TEST);

    FrameBaker fb(&window);
    gapi.EnableBlending();
    gapi.BlendFunc(GAPI::BLEND_PARAM::SRC_ALPHA, GAPI::BLEND_PARAM::ONE_MINUS_SRC_ALPHA);

    while (window.IsOpen()) {
        cam.SetRenderRange(0, (float) window.GetWidth(), (float) window.GetHeight(), 0, -1000, 1000);
        res_cam.SetRenderRange(0, (float) window.GetWidth(), 0, (float) window.GetHeight(), -1000, 1000);

        if (window.KeyIsPressed(GLFW_KEY_A)) transition.x -= spd;
        if (window.KeyIsPressed(GLFW_KEY_D)) transition.x += spd;
        if (window.KeyIsPressed(GLFW_KEY_W)) transition.y -= spd;
        if (window.KeyIsPressed(GLFW_KEY_S)) transition.y += spd;
        if (window.KeyIsPressed(GLFW_KEY_I)) transition.z -= spd;
        if (window.KeyIsPressed(GLFW_KEY_K)) transition.z += spd;

        cam.SetPosition(-transition);
        cam.SetRotation(transition.z);

        draw.Start();

        draw.SetCamera(&cam);

        draw.DrawMesh(quad, &bmb);

        draw.BeginBake(&fb);
        draw.DrawText(text2D);
        draw.EndBake();

        draw.SetCamera(&res_cam);
        draw.DrawBakedTexture(screenMesh, fb);

        draw.End();
    }

    return 0;
}
