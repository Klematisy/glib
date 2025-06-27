#include "glib.h"
#include <chrono>

#include <vector>

int main() {
    GlCore::Window window(1024, 768, "VLAD");

    glib::Draw draw(window);

    GlCore::Texture texture;
    texture.LoadImage("resources/images/Gangsters_1_Spritelist.png");

    glm::vec2 transition(0.0f, 0.0f);
    float speed = 5.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int i = 0;

    auto start = std::chrono::high_resolution_clock::now();

    while (window.IsOpen()) {
        draw.Start();

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
            transition.y -= speed;
        }

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
            transition.y += speed;
        }

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
            transition.x -= speed;
        }

        if (glfwGetKey(window.GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
            transition.x += speed;
        }

        draw.Quad(300, 500, 100.0f, {0.745f, 0.4f, 0.4f });
        draw.Quad(800, 100, 100.0f, {0.5f,   0.7f, 0.65f});
        draw.Texture({transition.x, transition.y, 200.0f, 200.0f}, {(float) 128 * i, 128 * 6, 128, 128}, &texture);


        if (i == 16) i = 0;

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = end - start;

        if (duration.count() >= 0.2f) {
            start = std::chrono::high_resolution_clock::now();
            i++;
        }

        draw.End();
    }

    return 0;
}