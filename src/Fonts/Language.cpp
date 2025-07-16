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





glib::LanguageTile::LanguageTile(uint32_t size, uint32_t count, int firstElement)
    :   m_Size(size), m_AtlasWidth(size * s_WidthCoefficient), m_AtlasHeight(size * s_HeightCoefficient),
        m_Count(count), m_FirstElementIndex(firstElement)
{
    FillFontPointer(count);
}

void glib::LanguageTile::FillFontPointer(uint32_t count) {
    m_TileCoordinates = std::make_unique<stbtt_bakedchar[]>(count);
}

void glib::LanguageTile::CreateAtlas(const unsigned char *font, wchar_t firstChar) {
    auto bitmap = (unsigned char*) malloc(m_AtlasWidth * m_AtlasHeight);
    stbtt_BakeFontBitmap(font,
                         0, (float) m_Size, bitmap,
                         m_AtlasWidth, m_AtlasHeight, firstChar, m_Count,
                         m_TileCoordinates.get());

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

stbtt_bakedchar *glib::LanguageTile::GetFontPointer() const {
    return m_TileCoordinates.get();
}

void glib::LanguageTile::GetSymbolQuad(float *xPos, float *yPos, wchar_t symbol, stbtt_aligned_quad *quad) {
    int index = symbol - m_FirstElementIndex;
    stbtt_GetBakedQuad(m_TileCoordinates.get(), (int) m_AtlasWidth, (int) m_AtlasHeight, index, xPos, yPos, quad, 1);
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

    m_LanguageTiles.emplace_back(size, GetCount(), m_FirstCharacter);
    auto& it = m_LanguageTiles.back();
    it.CreateAtlas((const unsigned char*) m_FontFile->get(), m_FirstCharacter);
    return it;
}
