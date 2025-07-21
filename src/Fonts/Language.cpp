#include "Font.h"

#include <vector>

glib::LangRange::LangRange(wchar_t firstChar, wchar_t lastChar)
    : m_FirstCharacter(firstChar), m_LastCharacter(lastChar)
{}

int glib::LangRange::GetCount() const {
    return m_LastCharacter - m_FirstCharacter + 1;
}

wchar_t glib::LangRange::GetFirstChar() const {
    return m_FirstCharacter;
}

wchar_t glib::LangRange::GetLastChar() const {
    return m_LastCharacter;
}




glib::LanguageData::LanguageData(const glib::LanguageData &other) noexcept
    : LangRange(other.m_FirstCharacter, other.m_LastCharacter),
      m_Id(other.m_Id)
{}

glib::LanguageData::LanguageData(int id, wchar_t firstChar, wchar_t lastChar)
    : LangRange(firstChar, lastChar),
      m_Id(id)
{}

const char *glib::LanguageData::GetName(int id) {
    switch (id) {
        case Language::ENG: return "ENG";
        case Language::RU:  return "RU";
        default:            return "";
    }
}

glib::LanguageData& glib::LanguageData::operator=(const glib::LanguageData &other) {
    m_Id = other.m_Id;
    m_FirstCharacter = other.m_FirstCharacter;
    m_LastCharacter = other.m_LastCharacter;
    return *this;
}

int glib::LanguageData::GetId() const {
    return m_Id;
}





glib::LanguageTile::LanguageTile(uint32_t size, const LangRange *langRange)
    :   m_Size(size),
    m_AtlasWidth(size * s_WidthCoefficient),
    m_AtlasHeight(size * s_HeightCoefficient),
    m_LangRange(langRange)
{
    FillFontPointer(langRange->GetCount());
}

void glib::LanguageTile::FillFontPointer(uint32_t count) {
    m_Glyphs = std::make_unique<GlyphInfo[]>(count);
}

void glib::LanguageTile::CreateAtlas(const unsigned char *fontFile) {
    auto bitmap = (unsigned char*) calloc(m_AtlasWidth * m_AtlasHeight, 1);

    stbtt_fontinfo font;
    stbtt_InitFont(&font, fontFile, stbtt_GetFontOffsetForIndex(fontFile, 0));

    int ascent, descent, lineGap;
    float scale, baseLine;

    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
    scale = stbtt_ScaleForPixelHeight(&font, (float) m_Size);
    baseLine = (float) ascent * scale;

    int penX = 0;
    int penY = 0;
    int rowHeight = 0;

    uint32_t firstChar = m_LangRange->GetFirstChar();
    uint32_t lastChar  = m_LangRange->GetLastChar();

    for (uint32_t i = firstChar; i < lastChar; i++) {
        int advance, lsb;
        int x0, y0, x1, y1;
        int s0, t0, s1, t1;

        stbtt_GetCodepointHMetrics(&font, (int) i, &advance, &lsb);
        stbtt_GetCodepointBitmapBox(&font, (int) i, scale, scale, &x0, &y0, &x1, &y1);

        int glyphW = x1 - x0;
        int glyphH = y1 - y0;

        if (penX + glyphW >= m_AtlasWidth) {
            penX = 0;
            penY += rowHeight + 1;
            rowHeight = 0;
        }

        stbtt_MakeCodepointBitmap(&font,
                                  bitmap + penY * m_AtlasWidth + penX,
                                  glyphW, glyphH, m_AtlasWidth,
                                  scale, scale,
                                  (int) i);

        s0 = penX;
        s1 = penX + glyphW;
        t0 = penY;
        t1 = penY + glyphH;

        m_Glyphs[i - firstChar] = {glyphW, glyphH,
                                   x0, (int) baseLine + y1,
                                   advance * scale,
                                   (float) s0 / m_AtlasWidth, (float) t0 / m_AtlasHeight,
                                   (float) s1 / m_AtlasWidth, (float) t1 / m_AtlasHeight};

        penX += glyphW + 1;

        if (glyphH > rowHeight) rowHeight = glyphH;
    }

    m_FontAtlas = std::make_unique<GlCore::Texture>(GlCore::Texture());
    m_FontAtlas->LoadImage(m_AtlasWidth, m_AtlasHeight, bitmap);

    free(bitmap);
}

int glib::LanguageTile::GetSize() const {
    return m_Size;
}

int glib::LanguageTile::GetWidth() const {
    return m_AtlasWidth;
}

int glib::LanguageTile::GetHeight() const {
    return m_AtlasHeight;
}

const GlCore::Texture &glib::LanguageTile::GetTexture() const {
    return *m_FontAtlas;
}

void glib::LanguageTile::GetSymbolQuad(float *x, float *y, wchar_t symbol, stbtt_aligned_quad *quad) {
    int index = symbol - m_LangRange->GetFirstChar();
    auto &glyph = m_Glyphs[index];

    quad->x0 = *x + glyph.xOffset;
    quad->x1 = quad->x0 + glyph.width;
    quad->y0 = *y - glyph.yOffset;
    quad->y1 = quad->y0 + glyph.height;

    quad->s0 = glyph.s0;
    quad->s1 = glyph.s1;
    quad->t0 = glyph.t0;
    quad->t1 = glyph.t1;

    *x += glyph.advance;
}


glib::LanguageTileSet::LanguageTileSet(std::unique_ptr<char[]> *fontFile, wchar_t firstChar, wchar_t lastChar)
    :   LangRange(firstChar, lastChar), m_FontFile(fontFile)
{}

glib::LanguageTileSet::LanguageTileSet(glib::LanguageTileSet &&other) noexcept
    : LangRange(other.m_FirstCharacter, other.m_LastCharacter), m_FontFile(other.m_FontFile)
{
    other.m_FirstCharacter = ' ';
    other.m_LastCharacter  = ' ';
    other.m_FontFile = nullptr;

    m_LanguageTiles = std::move(other.m_LanguageTiles);
}

glib::LanguageTile &glib::LanguageTileSet::GetTile(uint32_t size) {
    for (auto &it : m_LanguageTiles) {
        if (it.GetSize() == size) {
            return it;
        }
    }

    m_LanguageTiles.emplace_back(size, this);
    auto& it = m_LanguageTiles.back();
    it.CreateAtlas((const unsigned char*) m_FontFile->get());
    return it;
}
