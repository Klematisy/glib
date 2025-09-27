#include <iostream>
#include <Draw/draw.h>
#include <Geometry/mesh.h>

static GlCore::Window window(640, 640, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    Draw draw(window);
    Texture tex("resources/images/wonam.jpg");

    Geom::Mesh quad = Geom::MeshFactory::Get().CreateMesh("quad");

    float scale = 0.25f;
    quad.SetScale({tex.GetWidth() * scale, tex.GetHeight() * scale, 1});
    glm::vec3 rotation(0.0f);
    glm::vec3 pos {0.0f, 0.0f, 0.0f};
    float speed = 0.02f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (window.IsOpen()) {
        draw.Start();

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_J) == GLFW_PRESS) rotation.x -= 0.6f;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_L) == GLFW_PRESS) rotation.x += 0.6f;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_I) == GLFW_PRESS) rotation.y -= 0.6f;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_K) == GLFW_PRESS) rotation.y += 0.6f;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_U) == GLFW_PRESS) rotation.z -= 0.6f;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_O) == GLFW_PRESS) rotation.z += 0.6f;

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) pos.y -= speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) pos.y += speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) pos.x -= speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) pos.x += speed;


        quad.SetPosition(pos);
        draw.DrawMesh(quad, {1.0f, 1.0f, 1.0f}, &tex);
        quad.SetRotation(rotation);

        draw.End();
    }

    return 0;
}