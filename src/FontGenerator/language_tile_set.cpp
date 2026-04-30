#include "font_generator.h"

VLADLIB_NAMESPACE_OPEN

const FontTileInfo& LanguageTileSet::GetFontTileInfo(uint32_t size, msdfgen::FontHandle* m_Font) {
    if (m_Tiles.find(size) == m_Tiles.cend()) {
        m_Tiles[size] = std::make_unique<FontTile>();
        m_Tiles[size]->SetSize(size);
        m_Tiles[size]->CreateAtlas(m_Charset, m_Font);
    }

    return m_Tiles[size]->GetFontTileInfo();
}

int LanguageTileSet::GetFirstSymbol() const {
    return (int) *m_Charset.begin();
}

int LanguageTileSet::GetLastSymbol() const {
    return (int) (*m_Charset.begin() + m_Charset.size());
}

void LanguageTileSet::SetCharset(const Charset& charset) {
    m_Charset = charset;
}

VLADLIB_NAMESPACE_CLOSE