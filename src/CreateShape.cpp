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

    std::array<Vertex, 4> CreateShape::Letter(float x, float y, float size, wchar_t symbol, const LanguageTile& languageTile, int slot) {
        std::array<Vertex, 4> letter;

        int letterIndex = symbol - languageTile.GetFirstChar();
        auto &tile = languageTile.GetFontPointer()[letterIndex];

        float tw = languageTile.GetTexture().GetWidth();
        float th = languageTile.GetTexture().GetHeight();

        size /= 10;

        y = m_Window->GetHeight() - y - m_YLetterOffset;

        x += m_XLetterOffset;

        float width  = (float) (abs(tile.x0 - tile.x1)) * size;
        float height = (float) (abs(tile.y0 - tile.y1)) * size;

        m_XLetterOffset += tile.xadvance * size;

        letter[0] = {.position = {x,         y,          1.0f}, .texCoords = {(float) tile.x0 / tw, (float) tile.y1 / th}, .texSlot = (float) slot};
        letter[1] = {.position = {x,         y + height, 1.0f}, .texCoords = {(float) tile.x0 / tw, (float) tile.y0 / th}, .texSlot = (float) slot};
        letter[2] = {.position = {x + width, y + height, 1.0f}, .texCoords = {(float) tile.x1 / tw, (float) tile.y0 / th}, .texSlot = (float) slot};
        letter[3] = {.position = {x + width, y,          1.0f}, .texCoords = {(float) tile.x1 / tw, (float) tile.y1 / th}, .texSlot = (float) slot};

        return letter;
    }

    void CreateShape::ClearXLetterOffset() {
        m_XLetterOffset = 0;
    }

    void CreateShape::ClearYLetterOffset() {
        m_YLetterOffset = 0;
    }

    void CreateShape::SetYLetterOffset(float yLetterOffset) {
        m_YLetterOffset += yLetterOffset;
    }
}