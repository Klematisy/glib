#include "glib.h"

#include <array>

GLIB_NAMESPACE_OPEN

std::array<Vertex, 4> CreateShape::Rect(float x, float y, float width, float height, float angleD, Color color, const TexInfo& tex) {
    std::array<Vertex, 4> rect;

    y = m_Window->GetHeight() - y;
    height = -height;

    glm::vec2 center(x + width / 2, y + height / 2);
    rect[0] = {.position = {center.x -  x,          center.y - (y + height), 0.0f}, .color = color, .texCoords = {(float) tex.GetXOffset()                             / TexInfo::WIDTH_MAX_SIZE, (float)(tex.GetYOffset() + tex.GetTex()->GetWidth()) / TexInfo::HEIGHT_MAX_SIZE, tex.GetSlot()}};
    rect[1] = {.position = {center.x -  x,          center.y -  y          , 0.0f}, .color = color, .texCoords = {(float) tex.GetXOffset()                             / TexInfo::WIDTH_MAX_SIZE, (float) tex.GetYOffset()                             / TexInfo::HEIGHT_MAX_SIZE, tex.GetSlot()}};
    rect[2] = {.position = {center.x - (x + width), center.y -  y          , 0.0f}, .color = color, .texCoords = {(float)(tex.GetXOffset() + tex.GetTex()->GetWidth()) / TexInfo::WIDTH_MAX_SIZE, (float) tex.GetYOffset()                             / TexInfo::HEIGHT_MAX_SIZE, tex.GetSlot()}};
    rect[3] = {.position = {center.x - (x + width), center.y - (y + height), 0.0f}, .color = color, .texCoords = {(float)(tex.GetXOffset() + tex.GetTex()->GetWidth()) / TexInfo::WIDTH_MAX_SIZE, (float)(tex.GetYOffset() + tex.GetTex()->GetWidth()) / TexInfo::HEIGHT_MAX_SIZE, tex.GetSlot()}};

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

std::array<Vertex, 4> CreateShape::RectTex(float x, float y, float width, float height, float angleD, const TexInfo& tex) {
    std::array<Vertex, 4> rect;

    y = m_Window->GetHeight() - y;
    height = -height;

    glm::vec2 center(x + width / 2, y + height / 2);

    rect[0] = {.position = {center.x - (x + width), center.y - (y + height), 0.0f}, .texCoords = {(float)(tex.GetXOffset())                            / TexInfo::WIDTH_MAX_SIZE, (float)(tex.GetYOffset()                            ) / TexInfo::HEIGHT_MAX_SIZE, tex.GetSlot()}};
    rect[1] = {.position = {center.x - (x + width), center.y -  y          , 0.0f}, .texCoords = {(float)(tex.GetXOffset())                            / TexInfo::WIDTH_MAX_SIZE, (float)(tex.GetYOffset() + tex.GetTex()->GetHeight()) / TexInfo::HEIGHT_MAX_SIZE, tex.GetSlot()}};
    rect[2] = {.position = {center.x -  x,          center.y -  y          , 0.0f}, .texCoords = {(float)(tex.GetXOffset() + tex.GetTex()->GetWidth()) / TexInfo::WIDTH_MAX_SIZE, (float)(tex.GetYOffset() + tex.GetTex()->GetHeight()) / TexInfo::HEIGHT_MAX_SIZE, tex.GetSlot()}};
    rect[3] = {.position = {center.x -  x,          center.y - (y + height), 0.0f}, .texCoords = {(float)(tex.GetXOffset() + tex.GetTex()->GetWidth()) / TexInfo::WIDTH_MAX_SIZE, (float)(tex.GetYOffset()                            ) / TexInfo::HEIGHT_MAX_SIZE, tex.GetSlot()}};

    float angleInRadians = glm::radians(fmodf(angleD, 360));

    for (Vertex &it : rect) {
        glm::vec2 itPos = {it.position.x, it.position.y};
        it.position.x = center.x + itPos.x * glm::cos(angleInRadians) - itPos.y * glm::sin(angleInRadians);
        it.position.y = center.y + itPos.x * glm::sin(angleInRadians) + itPos.y * glm::cos(angleInRadians);
    }

    return rect;
}

std::array<Vertex, 4> CreateShape::RectTex(const Rectangle &objProperties, const Rectangle &texProperties, float angleD, const TexInfo& tex) {
    std::array<Vertex, 4> rect;

    auto o = &objProperties;
    auto t = texProperties;

    t.x += tex.GetXOffset();
    t.y += tex.GetYOffset();

    float y = m_Window->GetHeight() - o->y;
    float height = - o->height;


    float s0 = (t.x)           / TexInfo::WIDTH_MAX_SIZE;
    float s1 = (t.x + t.width) / TexInfo::WIDTH_MAX_SIZE;

    float g0 =  t.y             / TexInfo::HEIGHT_MAX_SIZE;
    float g1 = (t.y + t.height) / TexInfo::HEIGHT_MAX_SIZE;

    glm::vec2 center(o->x + o->width / 2, y + height / 2);
    rect[0] = {.position = glm::vec3(center.x - (o->x  + o->width), center.y - (y + height), 0.0f), .texCoords = {s0, g0, tex.GetSlot()}};
    rect[1] = {.position = glm::vec3(center.x - (o->x  + o->width), center.y -  y          , 0.0f), .texCoords = {s0, g1, tex.GetSlot()}};
    rect[2] = {.position = glm::vec3(center.x -  o->x,              center.y -  y          , 0.0f), .texCoords = {s1, g1, tex.GetSlot()}};
    rect[3] = {.position = glm::vec3(center.x -  o->x,              center.y - (y + height), 0.0f), .texCoords = {s1, g0, tex.GetSlot()}};

    float angleInRadians = glm::radians(fmodf(angleD, 360));

    for (Vertex &it : rect) {
        glm::vec2 itPos = {it.position.x, it.position.y};
        it.position.x = center.x + itPos.x * glm::cos(angleInRadians) - itPos.y * glm::sin(angleInRadians);
        it.position.y = center.y + itPos.x * glm::sin(angleInRadians) + itPos.y * glm::cos(angleInRadians);
    }

    return rect;
}

std::array<Vertex, 4> CreateShape::Letter(float *x, float *y, const glm::vec2& midPoint, float angleInRadians, wchar_t symbol, LanguageTile& tile, const TexInfo& tex) {
    std::array<Vertex, 4> letter;

    stbtt_aligned_quad quad;
    tile.GetSymbolQuad(x, *y, symbol, &quad);

    float kw = (float) tex.GetTex()->GetWidth()  / TexInfo::WIDTH_MAX_SIZE;
    float kh = (float) tex.GetTex()->GetHeight() / TexInfo::HEIGHT_MAX_SIZE;

    quad.s0 = quad.s0 * kw + ((float) tex.GetXOffset() / (float) TexInfo::WIDTH_MAX_SIZE);
    quad.s1 = quad.s1 * kw + ((float) tex.GetXOffset() / (float) TexInfo::WIDTH_MAX_SIZE);
    quad.t0 = quad.t0 * kh + ((float) tex.GetYOffset() / (float) TexInfo::HEIGHT_MAX_SIZE);
    quad.t1 = quad.t1 * kh + ((float) tex.GetYOffset() / (float) TexInfo::HEIGHT_MAX_SIZE);

    letter[0] = {.position = {quad.x0 - midPoint.x, quad.y0 - midPoint.y, 1.0f}, .texCoords = {(float) quad.s0, (float) quad.t1, (float) tex.GetSlot()}};
    letter[1] = {.position = {quad.x0 - midPoint.x, quad.y1 - midPoint.y, 1.0f}, .texCoords = {(float) quad.s0, (float) quad.t0, (float) tex.GetSlot()}};
    letter[2] = {.position = {quad.x1 - midPoint.x, quad.y1 - midPoint.y, 1.0f}, .texCoords = {(float) quad.s1, (float) quad.t0, (float) tex.GetSlot()}};
    letter[3] = {.position = {quad.x1 - midPoint.x, quad.y0 - midPoint.y, 1.0f}, .texCoords = {(float) quad.s1, (float) quad.t1, (float) tex.GetSlot()}};

    for (Vertex &it : letter) {
        glm::vec2 itPos = {it.position.x, it.position.y};
        it.position.x = midPoint.x + itPos.x * glm::cos(angleInRadians) - itPos.y * glm::sin(angleInRadians);
        it.position.y = midPoint.y + itPos.x * glm::sin(angleInRadians) + itPos.y * glm::cos(angleInRadians);
    }

    return letter;
}

glm::vec2 CreateShape::GetTextCenter(const std::wstring& text, struct Quad quad, const std::vector<LanguageTileSet>& tileSet) {
    float startX = quad.x, startY = quad.y;
    float endX   = quad.x, endY   = quad.y;
    endY += quad.size;

    float maxX = startX;
    for (wchar_t letter : text) {
        if (letter == L'\n') {
            endY += quad.size;
            maxX = std::max(maxX, endX);
            endX = startX;
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

GLIB_NAMESPACE_CLOSE