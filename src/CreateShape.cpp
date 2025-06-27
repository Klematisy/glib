#include "glib.h"

namespace glib {

    std::array<Vertex, 4> CreateShape::Rect(float x, float y, float width, float height, Color color) {
        std::array<Vertex, 4> rect;

        y = m_Window->GetHeight() - y;
        height = -height;

        rect[0] = {glm::vec3(x,         y + height, 0.0f), color};
        rect[1] = {glm::vec3(x,         y,          0.0f), color};
        rect[2] = {glm::vec3(x + width, y,          0.0f), color};
        rect[3] = {glm::vec3(x + width, y + height, 0.0f), color};

        return rect;
    }

    std::array<uint32_t, 6> CreateShape::RectangleIndices() {
        std::array<uint32_t, 6> indices {
            0, 1, 2,
            2, 3, 0
        };

        return indices;
    }

    std::array<Vertex, 4> CreateShape::RectTex(float x, float y, float width, float height) {
        std::array<Vertex, 4> rect;

        y = m_Window->GetHeight() - y;
        height = -height;

        rect[0] = {.position = glm::vec3(x,         y + height, 0.0f), .texCoords = glm::vec2(0.0f, 0.0f)};
        rect[1] = {.position = glm::vec3(x,         y,          0.0f), .texCoords = glm::vec2(0.0f, 1.0f)};
        rect[2] = {.position = glm::vec3(x + width, y,          0.0f), .texCoords = glm::vec2(1.0f, 1.0f)};
        rect[3] = {.position = glm::vec3(x + width, y + height, 0.0f), .texCoords = glm::vec2(1.0f, 0.0f)};

        return rect;
    }

    std::array<Vertex, 4> CreateShape::RectTex(const Rectangle &objProperties, const Rectangle &texProperties, int texWidth, int texHeight) {
        std::array<Vertex, 4> rect;

        auto o = &objProperties;
        auto t = texProperties;

        t.height *= -1;
        t.y = texHeight - t.y;

        float y = m_Window->GetHeight() - o->y;
        float height = - o->height;

        rect[0] = {.position = glm::vec3(o->x,            y + height, 0.0f), .texCoords = glm::vec2( t.x            / texWidth, (t.y + t.height) / texHeight)};
        rect[1] = {.position = glm::vec3(o->x,            y,          0.0f), .texCoords = glm::vec2 (t.x            / texWidth,  t.y             / texHeight)};
        rect[2] = {.position = glm::vec3(o->x + o->width, y,          0.0f), .texCoords = glm::vec2((t.x + t.width) / texWidth,  t.y             / texHeight)};
        rect[3] = {.position = glm::vec3(o->x + o->width, y + height, 0.0f), .texCoords = glm::vec2((t.x + t.width) / texWidth, (t.y + t.height) / texHeight)};

        return rect;
    }
}