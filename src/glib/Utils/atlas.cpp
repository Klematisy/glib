#include "atlas.h"

GLIB_NAMESPACE_OPEN

Atlas::Atlas(uint32_t cellWidth, uint32_t cellHeight)
    : m_CellWidth(cellWidth), m_CellHeight(cellHeight)
{}

void Atlas::PushBack(const std::string &state, uint32_t row_sprites_count, uint32_t y_position_in_atlas) {
    m_Rows[state] = {row_sprites_count, y_position_in_atlas};
}

Rectangle Atlas::Get(const std::string &state, int number) {
    if (m_Rows.find(state) == m_Rows.cend()) {
        Logger::LogErr("ATLAS", "This state doesn't exist!");
        return {};
    }

    number %= m_Rows[state].spritesCount;

    float x = (float)(abs(number) * m_CellWidth);
    float y = (float)(m_Rows[state].y * m_CellHeight);
    float width  = (float)m_CellWidth;
    float height = (float)m_CellHeight;

    if (number < 0) {
        x += m_CellWidth;
        width *= -1;
    }

    return {x, y,
            width, height};
}


GLIB_NAMESPACE_CLOSE