#include "Geometry/mesh.h"
#include "EntityAndComponents/entity.h"
#include "glib/Draw/draw.h"
#include "glib/Utils/atlas.h"
#include <thread>
#include <chrono>

static GlCore::Window window(1024, 768, "VLAD");

int main() {
    GLIB_NAMESPACE_USING;
    using namespace std::chrono_literals;

    /*
     *glib::Drawer drawer();
     *auto scene = glib::Scene::CreateScene("scene 1");
     */

    auto& meshFactory = Mesh::MeshFactory::Get();
    Mesh::Mesh m = meshFactory.CreateMesh("quad");

    ECS::Entity entity;
    entity.AddComponent<ECS::MeshComponent>(m);
    entity.AddComponent<ECS::Position>(3, 2);

    Draw draw(window);

    Texture texture("resources/images/Gangsters_1_Spritelist.png");
    Texture block("resources/images/gayBlock.png");

    glm::vec3 transition(2.0f, 4.0f, 0.51f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader("resources/shaders/user.glsl");

    auto start = std::chrono::high_resolution_clock::now();

    while (window.IsOpen()) {
        draw.Start();

        draw.Texture({500, 300, 100, 100}, 0.0f, &block);

        draw.Quad({0,   0,   100.0f},  0.0f, {0.5f, 0.7f, 0.65f});
        draw.Quad({924, 668, 100.0f}, 45.0f, {0.5f, 0.7f, 0.65f});

        // draw.Entity(quad);

        // draw.Entity(quad, texture);

        // draw.Entity(quad, shader);

        // draw.Entity(quad, shader, texture);

        draw.UseShader(shader);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> dur = end - start;

            shader.GetShader().SetUniform1f("u_Time", dur.count());
            draw.Text(L"Hello", {400, 400, 4}, 0.0f, {1.0f, 1.0f, 1.0f});
        draw.UnUseShader();


        if (glfwGetKey(window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
            GlCore::ShaderCache::GetCache().HotReload();
            std::this_thread::sleep_for(200ms);
        }

        draw.End();
    }

    return 0;
}