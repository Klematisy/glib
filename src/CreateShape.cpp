#include "glib.h"

namespace glib {

    std::array<Vertex, 4> CreateShape::Rect(float x, float y, float width, float height, float angleD, Color color, int slot) {
        std::array<Vertex, 4> rect;

        y = m_Window->GetHeight() - y;
        height = -height;
        
        glm::vec2 center(x + width / 2, y + height / 2);
        rect[0] = {.position = glm::vec3(center.x -  x,          center.y - (y + height), 0.0f), .color = color, .texSlot = (float) slot};
        rect[1] = {.position = glm::vec3(center.x -  x,          center.y -  y          , 0.0f), .color = color, .texSlot = (float) slot};
        rect[2] = {.position = glm::vec3(center.x - (x + width), center.y -  y          , 0.0f), .color = color, .texSlot = (float) slot};
        rect[3] = {.position = glm::vec3(center.x - (x + width), center.y - (y + height), 0.0f), .color = color, .texSlot = (float) slot};

        float angleInRadians = glm::radians(fmodf(angleD, 360));

        for (Vertex &it : rect) {
            glm::vec2 itPos = {it.position.x, it.position.y};
            it.position.x = center.x + itPos.x * glm::cos(angleInRadians) - itPos.y * glm::sin(angleInRadians);
            it.position.y = center.y + itPos.x * glm::sin(angleInRadians) + itPos.y * glm::cos(angleInRadians);
        }

        return rect;
    }

    std::array<uint32_t, 6> CreateShape::RectangleIndices() {
        std::array<uint32_t, 6> indices {
            0, 1, 2,
            2, 3, 0
        };

        return indices;
    }

    std::array<Vertex, 4> CreateShape::RectTex(float x, float y, float width, float height, float angleD, int slot) {
        std::array<Vertex, 4> rect;

        y = m_Window->GetHeight() - y;
        height = -height;

        glm::vec2 center(x + width / 2, y + height / 2);
        rect[0] = {.position = glm::vec3(center.x - (x + width), center.y - (y + height), 0.0f), .texCoords = glm::vec2(0.0f, 1.0f), .texSlot = (float) slot};
        rect[1] = {.position = glm::vec3(center.x - (x + width), center.y -  y          , 0.0f), .texCoords = glm::vec2(0.0f, 0.0f), .texSlot = (float) slot};
        rect[2] = {.position = glm::vec3(center.x -  x,          center.y -  y          , 0.0f), .texCoords = glm::vec2(1.0f, 0.0f), .texSlot = (float) slot};
        rect[3] = {.position = glm::vec3(center.x -  x,          center.y - (y + height), 0.0f), .texCoords = glm::vec2(1.0f, 1.0f), .texSlot = (float) slot};

        float angleInRadians = glm::radians(fmodf(angleD, 360));

        for (Vertex &it : rect) {
            glm::vec2 itPos = {it.position.x, it.position.y};
            it.position.x = center.x + itPos.x * glm::cos(angleInRadians) - itPos.y * glm::sin(angleInRadians);
            it.position.y = center.y + itPos.x * glm::sin(angleInRadians) + itPos.y * glm::cos(angleInRadians);
        }

        return rect;
    }

    std::array<Vertex, 4> CreateShape::RectTex(const Rectangle &objProperties, const Rectangle &texProperties, float angleD, int texWidth, int texHeight, int slot) {
        std::array<Vertex, 4> rect;

        auto o = &objProperties;
        auto t = texProperties;

        t.y = texHeight - t.y;
        t.height *= -1;

        float y = m_Window->GetHeight() - o->y;
        float height = - o->height;


        glm::vec2 center(o->x + o->width / 2, y + height / 2);
        rect[0] = {.position = glm::vec3(center.x - (o->x  + o->width), center.y - (y + height), 0.0f), .texCoords = glm::vec2( t.x            / texWidth,  t.y             / texHeight), .texSlot = (float) slot};
        rect[1] = {.position = glm::vec3(center.x - (o->x  + o->width), center.y -  y          , 0.0f), .texCoords = glm::vec2 (t.x            / texWidth, (t.y + t.height) / texHeight), .texSlot = (float) slot};
        rect[2] = {.position = glm::vec3(center.x -  o->x,              center.y -  y          , 0.0f), .texCoords = glm::vec2((t.x + t.width) / texWidth, (t.y + t.height) / texHeight), .texSlot = (float) slot};
        rect[3] = {.position = glm::vec3(center.x -  o->x,              center.y - (y + height), 0.0f), .texCoords = glm::vec2((t.x + t.width) / texWidth,  t.y             / texHeight), .texSlot = (float) slot};

        float angleInRadians = glm::radians(fmodf(angleD, 360));

        for (Vertex &it : rect) {
            glm::vec2 itPos = {it.position.x, it.position.y};
            it.position.x = center.x + itPos.x * glm::cos(angleInRadians) - itPos.y * glm::sin(angleInRadians);
            it.position.y = center.y + itPos.x * glm::sin(angleInRadians) + itPos.y * glm::cos(angleInRadians);
        }

        return rect;
    }

    std::array<Vertex, 4> CreateShape::Letter(float *x, float *y, const glm::vec2& midPoint, float angleInRadians, wchar_t symbol, LanguageTile& tile, int slot) {
        std::array<Vertex, 4> letter;

        stbtt_aligned_quad quad;
        tile.GetSymbolQuad(x, *y, symbol, &quad);

        letter[0] = {.position = {quad.x0 - midPoint.x, quad.y0 - midPoint.y, 1.0f}, .texCoords = {(float) quad.s0, (float) quad.t1}, .texSlot = (float) slot};
        letter[1] = {.position = {quad.x0 - midPoint.x, quad.y1 - midPoint.y, 1.0f}, .texCoords = {(float) quad.s0, (float) quad.t0}, .texSlot = (float) slot};
        letter[2] = {.position = {quad.x1 - midPoint.x, quad.y1 - midPoint.y, 1.0f}, .texCoords = {(float) quad.s1, (float) quad.t0}, .texSlot = (float) slot};
        letter[3] = {.position = {quad.x1 - midPoint.x, quad.y0 - midPoint.y, 1.0f}, .texCoords = {(float) quad.s1, (float) quad.t1}, .texSlot = (float) slot};

        for (Vertex &it : letter) {
            glm::vec2 itPos = {it.position.x, it.position.y};
            it.position.x = midPoint.x + itPos.x * glm::cos(angleInRadians) - itPos.y * glm::sin(angleInRadians);
            it.position.y = midPoint.y + itPos.x * glm::sin(angleInRadians) + itPos.y * glm::cos(angleInRadians);
        }

        return letter;
    }

    glm::vec2 CreateShape::GetTextCenter(const std::wstring& text, struct Quad quad, const std::vector<LanguageTileSet>& tileSet) {
        float startX = quad.x, startY = quad.y;
        float endX   = 0.0f,   endY   = 0.0f;
        endY += quad.size;

        for (wchar_t letter : text) {
            if (letter == L'\n') {
                endY += quad.size;
                continue;
            }
            for (auto & langTile : tileSet) {
                if (langTile.GetFirstChar() <= letter && letter <= langTile.GetLastChar()) {
                    auto &tile = langTile.GetTile((uint32_t) quad.size);
                    tile.GetSymbolQuad(&endX, endY, letter, nullptr);
                }
            }
        }

        return {quad.x + (endX - startX) / 2, quad.y + (endY - startY) / 2};
    }
}