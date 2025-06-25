#include "glib.h"

int main() {
    GlCore::Window window(1024, 768, "VLAD");

    glib::Draw draw(window);

    while (window.IsOpen()) {
        draw.Start();

        draw.Quad(300, 500, 100.0f, {0.745f, 0.4f, 0.4f });
        draw.Quad(800, 100, 100.0f, {0.5f,   0.7f, 0.65f});

        draw.End();
    }

    return 0;
}