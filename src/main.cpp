#include <iostream>
#include <thread>
#include <Draw/draw.h>
#include <Geometry/mesh.h>

static GlCore::Window window(640, 640, "glib");

int main() {
    GLIB_NAMESPACE_USING;

    using namespace std::chrono_literals;

    Draw draw(window);
    Texture tex("resources/images/wonam.jpg");
    Shader shader("resources/shaders/user.glsl");

    Geom::Mesh quad = Geom::MeshFactory::Get().CreateMesh("quad");
    quad.SetScale({tex.GetWidth() * 0.25f, tex.GetHeight() * 0.25f, 1});

    glm::vec3 rotation(0.0f);
    glm::vec3 pos {0.0f, 0.0f, 0.0f};
    float speed = 0.02f;


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto start = std::chrono::high_resolution_clock::now();

    float *point = &rotation.x;

    while (window.IsOpen()) {
        draw.Start();

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_X) == GLFW_PRESS) point = &rotation.x;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_Y) == GLFW_PRESS) point = &rotation.y;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_Z) == GLFW_PRESS) point = &rotation.z;

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)  *point -= 0.6f;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) *point += 0.6f;

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS) pos.y -= speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS) pos.y += speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) pos.x -= speed;
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) pos.x += speed;

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
            GlCore::ShaderCache::GetCache().HotReload();
            std::this_thread::sleep_for(200ms);
        }

        quad.SetPosition(pos);
        quad.SetRotation(rotation);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> dur = end - start;
        shader.GetShader().SetUniform1f("u_Time", dur.count());

        draw.DrawMesh(quad, {1.0f, 1.0f, 1.0f}, &tex, &shader);

        draw.End();
    }

    return 0;
}