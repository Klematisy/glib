#include "glib/Base/draw.h"
#include "glib/Utils/atlas.h"
#include <thread>
#include <chrono>

static GlCore::Window window(1024, 768, "VLAD");
static int i = 0;
bool horizontal_mirror = false;

std::string bro_state = "stay";

void input(glm::vec3& transition) {
    float speed  = 1.0f;

    bro_state = "stay";
    horizontal_mirror = false;

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed *= 3;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
        bro_state = "jump";
        transition.y -= speed;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        bro_state = "death";
        transition.y += speed;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        bro_state = "walk";
        if (speed > 1.0f) bro_state = "run";
        if (i > 0) {
            i = -i;
            horizontal_mirror = true;
        }
        transition.x -= speed;
    }

    if (glfwGetKey(window.GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        bro_state = "walk";
        if (speed > 1.0f) bro_state = "run";
        if (i < 0) {
            i = -i;
            horizontal_mirror = true;
        }
        transition.x += speed;
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glib::Shader shader("resources/shaders/user.glsl");

    auto start = std::chrono::high_resolution_clock::now();

    glib::Atlas mafia(128, 128);

    mafia.PushBack("jump",  10, 4);
    mafia.PushBack("run",   10, 3);
    mafia.PushBack("walk",  10, 2);
    mafia.PushBack("stay",  6,  0);
    mafia.PushBack("death", 5,  9);

    while (window.IsOpen()) {
        input(transition);

        draw.Start();

        draw.Texture({transition.x, transition.y, 200, 200}, mafia.Get(bro_state, i), 0.0f, &texture);

        draw.Quad({0,   0,   100.0f},  0.0f, {0.5f, 0.7f, 0.65f});
        draw.Quad({924, 668, 100.0f}, 45.0f, {0.5f, 0.7f, 0.65f});

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> dur = end - start;

        if (dur.count() > 0.1f) {
            start = std::chrono::high_resolution_clock::now();
            if (horizontal_mirror) i--;
            else i++;
        }

        draw.QTexture({500, 200, 200.0f}, 0.0f, &wonam);

        draw.End();
    }

    return 0;
}