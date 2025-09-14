#include "glib/Base/draw.h"
#include "glib/Utils/atlas.h"
#include <thread>

GlCore::Window window(1024, 768, "VLAD");

void input(glm::vec3& transition, float& m_Zoom, float& rotation) {
    float speed  = 1.0f;
    float zspeed = 0.01f;
    float rspeed = 1.00f;

    float k = fabsf(4.5f - m_Zoom);

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
        transition.y -= speed * k;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        transition.y += speed * k;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        transition.x -= speed * k;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        transition.x += speed * k;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS && m_Zoom < 5.0f) {
        m_Zoom += zspeed;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS && m_Zoom > 0.0f) {
        m_Zoom -= zspeed;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        rotation += rspeed;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        rotation -= rspeed;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
        GlCore::ShaderCache::GetCache().HotReload();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    glib::Draw draw(window);

    glib::Texture texture("resources/images/Gangsters_1_Spritelist.png");
    glib::Texture wonam("resources/images/wonam.jpg");
    glib::Texture block("resources/images/gayBlock.png");

    glm::vec3 transition(2.0f, 4.0f, 0.51f);
    float m_Zoom = 1.0f;
    float rotation = 0.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glib::Shader shader("resources/shaders/user.glsl");

    auto start = std::chrono::high_resolution_clock::now();

    glib::Atlas mafia(128, 128);

    mafia.PushBack("jump", 10, 4);
    mafia.PushBack("run",  10, 3);
    mafia.PushBack("walk", 10, 2);



    uint32_t i = 0;
    while (window.IsOpen()) {
        input(transition, m_Zoom, rotation);

        draw.Start();

        draw.Texture({transition.x, transition.y, 200, 200}, mafia.Get("walk", i), 0.0f, &texture);

        draw.Quad({0,   0,   100.0f},  0.0f, {0.5f, 0.7f, 0.65f});
        draw.Quad({924, 668, 100.0f}, 45.0f, {0.5f, 0.7f, 0.65f});

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> dur = end - start;

        draw.UseShader(shader);
            draw.QTexture({700, 500, 200.0f}, 0.0f, &block);
            shader.GetShader().SetUniform1f("u_Time", dur.count());
        draw.UnUseShader();

        draw.Text(L"Swaga\nonline", {0, 0, 4}, rotation, {255, 255, 255});
        if (dur.count() > 0.1f * (float)(i + 1)) {
            i++;
        }

        draw.QTexture({500, 200, 200.0f}, 0.0f, &wonam);


        draw.End();
    }

    return 0;
}