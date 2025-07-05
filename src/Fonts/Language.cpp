#include "Font.h"

glib::LanguageData::LanguageData(const glib::LanguageData &other) noexcept {
    m_Id             = other.m_Id;
    m_FirstCharacter = other.m_FirstCharacter;
    m_LastCharacter  = other.m_LastCharacter;
}

glib::LanguageData::LanguageData(int id, wchar_t firstChar, wchar_t lastChar)
    : m_Id(id), m_FirstCharacter(firstChar), m_LastCharacter(lastChar)
{}

const char *glib::LanguageData::GetName(int id) {
    switch (id) {
        case Language::ENG: return "ENG";
        case Language::RU:  return "RU";
        default:            return "";
    }
}

int glib::LanguageData::GetCount() const {
    return m_LastCharacter - m_FirstCharacter + 1;
}

wchar_t glib::LanguageData::GetFirstChar() const {
    return m_FirstCharacter;
}

wchar_t glib::LanguageData::GetLastChar() const {
    return m_LastCharacter;
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


glib::LanguageTile::LanguageTile(wchar_t firstChar, wchar_t lastChar)
        : LanguageData(0, firstChar, lastChar)
{}

glib::LanguageTile::LanguageTile(glib::LanguageTile &&other) noexcept {
    m_FirstCharacter = other.m_FirstCharacter;
    m_LastCharacter = other.m_LastCharacter;

    m_FontTileCoordinates = std::move(other.m_FontTileCoordinates);
    m_FontAtlas = std::move(other.m_FontAtlas);
}

stbtt_bakedchar *glib::LanguageTile::GetFontPointer() const {
    return m_FontTileCoordinates.get();
}

void glib::LanguageTile::FillFontPointer() {
    m_FontTileCoordinates = std::make_unique<stbtt_bakedchar[]>(GetCount());
}

void glib::LanguageTile::CreateTexture(const char *imagePath) {
    m_FontAtlas = std::make_unique<GlCore::Texture>(GlCore::Texture());
    m_FontAtlas->LoadImage(imagePath);
}

const GlCore::Texture &glib::LanguageTile::GetTexture() const {
    return *m_FontAtlas;
}
