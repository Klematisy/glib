#include "font_generator.h"
#include <utility>
#include <functional>

GLIB_NAMESPACE_OPEN

using namespace msdf_atlas;

static LanguageFactory& s_factory = LanguageFactory::Get();

LanguageFactory::LanguageFactory() {
    AddLang(LangId::RU, std::function<Charset()>([]() {
        Charset charset;
        for (int i = 1040; i < 1105; i++) {
            charset.add(i);
        }
        return charset;
    }));

    AddLang(LangId::ENG, std::function<Charset()>([]() {
        Charset charset;
        for (int i = 32; i < 122; i++) {
            charset.add(i);
        }
        return charset;
    }));
}

LanguageFactory& LanguageFactory::Get() {
    static LanguageFactory lf;
    return lf;
}

void LanguageFactory::AddLang(int langId, std::function<Charset()> func) {
    if (m_Tiles.find(langId) != m_Tiles.cend()) {
        Logger::LogErr("FONT", "THIS FONT ALREADY EXISTS");
        return;
    }

    m_Tiles[langId] = std::move(func);
}

Charset LanguageFactory::CreateTile(int langId) {
    if (m_Tiles.find(langId) == m_Tiles.cend()) {
        Logger::LogErr("FONT", "FATAL ERROR WITH FONT CREATING");
        m_Tiles.begin()->second(); // LanguageFactory always not empty
    }

    return m_Tiles[langId]();
}

Font::Font(int lang, const char* filePath) {
    SetFontFile(filePath);
    SetLanguages(lang);

    m_LanguageTiles[LangId::ENG]->GetFontTileInfo(50, m_Font);
}

void Font::SetFontFile(const char *filePath) {
    using namespace std::string_literals;

    m_Font = msdfgen::loadFont(m_Ft, filePath);

    if (m_Font)
        Logger::LogInf("FONT", "The font '"s + "' has loaded!");
    else
        Logger::LogErr("FONT", "The font '"s + "' hasn't loaded!");
}

void Font::SetLanguages(int lang) {
    using namespace std::string_literals;

    if (!m_Font) {
        Logger::LogErr("FONT", "The font '"s + "' hasn't loaded! SetLanguages(int lang) will abort");
        return;
    }

    for (int id : LangId::langIds) {
        if ((id & lang) == 0) continue;
        LoadLanguage(lang);
    }
}

void Font::LoadLanguage(int langId) {
    bool idExists = m_LanguageTiles.find(langId) != m_LanguageTiles.cend();

    if (!idExists) {
        m_LanguageTiles[langId] = std::make_unique<LanguageTileSet>();
        m_LanguageTiles[langId]->SetCharset(s_factory.CreateTile(langId));
    }
}

Font::~Font() {
    msdfgen::destroyFont(m_Font);
    msdfgen::deinitializeFreetype(m_Ft);
}

CharTileInfo Font::GetGlyph(char c, uint32_t size) const {
    for (auto &it : m_LanguageTiles) {
        auto &lang = it.second;
        char fs = lang->GetFirstSymbol();
        char ls = lang->GetLastSymbol();

        if (fs <= c && c <= ls) {
            const auto& info = lang->GetFontTileInfo(size, m_Font);
            return {&info.glyphs[c - fs], info.tex};
        }
    }

    Logger::LogWar("FONT_GETGLYPH", "This language doesn't exist!");
    return {};
}

GLIB_NAMESPACE_CLOSE
