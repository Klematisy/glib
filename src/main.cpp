#include "glib.h"
#include <chrono>
#include <thread>

GlCore::Window window(1024, 768, "VLAD");

void input(glm::vec3& transition, float& m_Zoom) {
    float speed = 5.0f;
    float zspeed = 0.01f;

    float k = fabsf(4.5f - m_Zoom);

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
        transition.y -= speed * k;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        transition.y += speed * k;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        transition.x += speed * k;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        transition.x -= speed * k;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_W) == GLFW_PRESS && m_Zoom < 5.0f) {
        m_Zoom += zspeed;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_S) == GLFW_PRESS && m_Zoom > 0.0f) {
        m_Zoom -= zspeed;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
        GlCore::ShaderCache::GetCache().HotReload();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {
    glib::Draw draw(window);

    GlCore::Texture texture;
    texture.LoadImage("resources/images/Gangsters_1_Spritelist.png");

    glm::vec3 transition(0.0f, 0.0f, 0.51f);
    float m_Zoom = 1.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    uint32_t i = 0;

    glib::Shader shader("resources/shaders/user.glsl");

    auto start = std::chrono::high_resolution_clock::now();

    while (window.IsOpen()) {
        input(transition, m_Zoom);

        draw.GetCamera().SetPosition({transition.x, transition.y});
        draw.GetCamera().SetZoom(m_Zoom);

        draw.Start();

        draw.UseShader(shader);
        
            draw.Quad(400, 300, 100.0f, {0.5f, 0.7f, 0.65f});

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> dur = end - start;
            shader.GetShader().SetUniform1f("u_Time", dur.count() / 2);

        draw.UnUseShader();

        draw.End();
    }

    return 0;
}