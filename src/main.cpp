#include "glib.h"
#include <chrono>
#include <thread>

GlCore::Window window(1024, 768, "VLAD");

void input(glm::vec3& transition, float& m_Zoom, float& rotation) {
    float speed = 3.0f;
    float zspeed = 0.01f;
    float rspeed = 1.0f;

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
    glib::Texture boy("resources/images/beautiful_minimalistic_boy.png");

    glm::vec3 transition(2.0f, 4.0f, 0.51f);
    float m_Zoom = 1.0f;
    float rotation = 0.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glib::Shader shader("resources/shaders/user.glsl");

    auto start = std::chrono::high_resolution_clock::now();

    while (window.IsOpen()) {
        input(transition, m_Zoom, rotation);

        draw.GetCamera().SetPosition({transition.x, transition.y});
        draw.GetCamera().SetZoom(m_Zoom);
        draw.GetCamera().SetRotation(0.0f);

        draw.Start();

        draw.Quad({0,   0,   100.0f},  0.0f, {0.5f, 0.7f, 0.65f});
        draw.Quad({924, 668, 100.0f}, 45.0f, {0.5f, 0.7f, 0.65f});

        draw.Texture({200, 200, 200, 200}, {128, 0, 128, 128}, 0.0f, &texture);

        draw.UseShader(shader);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> dur = end - start;
            shader.GetShader().SetUniformMatrix4fv("u_Proj", &draw.GetProjMatrix()[0][0]);
            shader.GetShader().SetUniform1f("u_Time", dur.count());

            draw.Text(L"Glib demo test", {0, 0, 3}, rotation, {1.0f, 1.0f, 1.0f});
        draw.UnUseShader();
        draw.QTexture({500, 200, 200.0f}, 0.0f, &boy);

#ifdef __GLIB_DEBUG__
        if (glfwGetKey(window.GetWindow(), GLFW_KEY_R) == GLFW_PRESS) {
            draw.m_TexManager.PrintTextures();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
#endif

        draw.End();
    }

    return 0;
}