#pragma once

#include <iostream>
#include <unordered_map>

#include "Base/structs.h"

GLIB_NAMESPACE_OPEN

class Atlas {
public:
    Atlas(uint32_t cellWidth, uint32_t cellHeight);
    void PushBack(const std::string& state, uint32_t row_sprites_count, uint32_t y_position_in_atlas);
    Rectangle Get(const std::string& state, int number);
private:
    struct RowCells {
        uint32_t spritesCount = 0;
        uint32_t y = 0;
    };

    uint32_t m_CellWidth = 0;
    uint32_t m_CellHeight = 0;

    std::unordered_map<std::string, RowCells> m_Rows;
};

GLIB_NAMESPACE_CLOSE