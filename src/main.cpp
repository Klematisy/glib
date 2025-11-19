#include "Draw/drawer.h"
#include "FontGenerator/font_generator.h"

static RendererCore::Window window(600, 600, "glib");

int main() {
    GLIB_NAMESPACE_USING;
    Drawer draw(window);

    Texture tex("resources/images/beautiful_minimalistic_boy.png");
    Font font("resources/Fonts/Helvetica.ttf");
    Geom::Text2D text("Lol_l", &font, 4);

    auto mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    mesh.SetScale({100, 100, 1.0f});

    gapi.EnableBlending();
    gapi.BlendFunc(GAPI::BLEND_PARAM::SRC_ALPHA, GAPI::BLEND_PARAM::ONE_MINUS_SRC_ALPHA);

    glm::vec3 transition(0.0f);
    float speed = 6.0f;

    while (window.IsOpen()) {
        draw.Start();

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)  transition.x -= speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) transition.x += speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_UP) == GLFW_PRESS) transition.y -= speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) transition.y += speed;
        text.WriteMesh()->SetPosition(transition);

        mesh.SetPosition({100, 100, 0});
        draw.DrawMesh(mesh, {1.0f, 1.0f, 1.0f, 1.0f}, &tex);
        draw.DrawText(text, {0.0f, 0.6f, 0.0f, 1.0f});

        mesh.SetPosition({20, 500, 0});
        draw.DrawMesh(mesh, {1.0f, 0.0f, 1.0f, 1.0f}, &tex);

        draw.End();
    }

    return 0;
}
