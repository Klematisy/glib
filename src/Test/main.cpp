#include "Draw/drawer.h"
#include "FontGenerator/font_generator.h"

static RendererCore::Window window(600, 600, "glib");

int main() {
    GLIB_NAMESPACE_USING;
    Drawer draw(window);
    Shader myLolShader;
    myLolShader.AddSrcFiles("src/Test/lol.glsl");
    myLolShader.Compile();

    Texture tex("resources/images/beautiful_minimalistic_boy.png");
    Font font("resources/Fonts/Helvetica.ttf");
    Geom::Text2D text("Lol_lLllabc", &font, 4);
    text.WriteMesh()->SetColor({
        {1.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f}
    });

    auto mesh = Geom::MeshFactory::Get().CreateMesh("quad");
    mesh.SetScale({150, 150, 0.0f});

    auto screenMesh = Geom::MeshFactory::Get().CreateMesh("quad");
    screenMesh.SetScale({600.0f, 600.0f, 0.0f});
    screenMesh.SetColor({
            {0.5f, 0.5f, 1.0f, 1.0f},
            {0.8f, 1.0f, 0.8f, 1.0f},
            {1.0f, 1.0f, 0.4f, 1.0f},
            {0.6f, 0.7f, 0.6f, 1.0f}
    });

    gapi.EnableBlending();
    gapi.BlendFunc(GAPI::BLEND_PARAM::SRC_ALPHA, GAPI::BLEND_PARAM::ONE_MINUS_SRC_ALPHA);

    glm::vec3 transition(0.0f);
    glm::vec3 t(0.0f);
    float speed = 5.0f;

    Camera camera(&window);
    draw.SetCamera(&camera);

    FrameBaker fm(&window);

    while (window.IsOpen()) {
        draw.Start();

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) t.y -= speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) t.y += speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) t.x -= speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) t.x += speed;

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT)  == GLFW_PRESS) transition.x += speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) transition.x -= speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_UP)    == GLFW_PRESS) transition.y += speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_DOWN)  == GLFW_PRESS) transition.y -= speed;

        screenMesh.SetScale({(int) window.GetWidth(), (int) window.GetHeight(), 0.0f});

        camera.SetPosition(transition);
        camera.UpdateView();

        draw.DrawText(text);
        draw.BeginBake(&fm, {0, 0, 600, 600});

        mesh.SetPosition({0.0f, 0.0f, 0.0f});
        draw.DrawMesh(mesh, &tex);

        mesh.SetPosition(t);
        draw.DrawMesh(mesh, &tex);
        draw.EndBake();

        screenMesh.SetPosition(-transition);
        draw.DrawBakedTexture(screenMesh, fm);

        draw.End();
    }

    return 0;
}
