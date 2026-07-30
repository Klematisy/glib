#pragma once

#include <fstream>
#include <memory>
#include <array>

#include "stb/stb_truetype.h"
#include "msdf-atlas-gen/msdf-atlas-gen.h"
#include "msdfgen-ext.h"

#include "Logger/logger.h"
#include "environment.h"
#include "GraphicsAPI/graphics_api.h"

VLADLIB_NAMESPACE_OPEN

using namespace msdf_atlas;

struct FontTileInfo {
    std::vector<GlyphGeometry> glyphs;
    GAPI::ImageInfo tex;
};

struct CharTileInfo {
    const GlyphGeometry* glyph = nullptr;
    const GAPI::ImageInfo* tex;
};

class FontTile {
public:
    FontTile() = default;

    void CreateAtlas(const Charset& chset, msdfgen::FontHandle* m_Font);

    uint32_t GetSize() const;
    const FontTileInfo& GetFontTileInfo() const;

    void SetSize(uint32_t size);
private:
    static constexpr uint32_t WIDTH_K  = 10;
    static constexpr uint32_t HEIGHT_K = 6;

    FontTileInfo m_FontTileInfo {};
    uint32_t m_Size = 0;
};

class LanguageTileSet {
public:
    LanguageTileSet() = default;
    LanguageTileSet(LanguageTileSet&&) = delete;
    LanguageTileSet& operator=(LanguageTileSet&& other) = delete;

    const FontTileInfo& GetFontTileInfo(uint32_t size, msdfgen::FontHandle* m_Font);
    int GetFirstSymbol() const;
    int GetLastSymbol() const;

    void SetCharset(const Charset& charset);
private:
    Charset m_Charset;

    std::unordered_map<uint32_t, std::unique_ptr<FontTile>> m_Tiles;
};

class LangId {
public:
    static constexpr int RU  = 1 << 0;
    static constexpr int ENG = 1 << 1;

    static constexpr std::array<int, 2> langIds {
        RU, ENG
    };
};

class LanguageFactory {
public:
    static LanguageFactory& Get();
    void AddLang(int langId, std::function<Charset()> func);
    Charset CreateTile(int langId);
private:
    LanguageFactory();

    std::unordered_map<int, std::function<Charset()>> m_Tiles;
};

class Font {
public:
    Font(const char* filePath = "", i32 lang = LangId::ENG);
    Font(const Font&) = default;
    Font(Font&&) = delete;
    ~Font();

    CharTileInfo GetGlyph(char c, u32 size) const;

    Font& operator=(const Font& other) = default;
    Font& operator=(Font&& other) = delete;
private:
    void LoadLanguage(int lang);

    msdfgen::FreetypeHandle* m_Ft = msdfgen::initializeFreetype();
    msdfgen::FontHandle* m_Font = nullptr;

    std::unordered_map<int, std::unique_ptr<LanguageTileSet>> m_LanguageTiles;
};


VLADLIB_NAMESPACE_CLOSE
