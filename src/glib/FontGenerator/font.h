#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <array>

#include "stb/stb_truetype.h"
#include "Logger/logger.h"

#include "environment.h"
#include "Draw/texture.h"

GLIB_NAMESPACE_OPEN

struct GlyphInfo {
    int width, height;
    int xOffset, yOffset;
    float advance;
    float s0, t0, s1, t1;
};

using Glyphs = std::vector<GlyphInfo>;

class FontTile {
public:
    FontTile() = default;
    FontTile(uint32_t size);
    FontTile(const FontTile& other);

    FontTile& operator=(const FontTile& other);

    void CreateAtlas(int firstSymbol, int lastSymbol, unsigned char* fontFile);

    uint32_t GetSize() const;
    const Glyphs& GetGlyphs() const;
    const Texture* GetTex() const;

    void SetSize(uint32_t size);
private:
    static constexpr uint32_t WIDTH_K  = 10;
    static constexpr uint32_t HEIGHT_K = 6;

    uint32_t m_Size = 0;
    Glyphs m_Glyphs;
    std::shared_ptr<Texture> m_Tex;
};

class LanguageTileSet {
public:
    LanguageTileSet() = default;
    LanguageTileSet(int firstSymbol, int lastSymbol);
    LanguageTileSet(const LanguageTileSet& other);
    LanguageTileSet& operator=(const LanguageTileSet& other);

    const Glyphs& GetFontTileInfo(uint32_t size);
    const Texture* GetFontTile(uint32_t size);
    int GetFirstSymbol() const;
    int GetLastSymbol() const;
    unsigned char* GetFontFile() const;

    void SetFirstSymbol(int fs);
    void SetLastSymbol(int ls);
    void SetFontFile(unsigned char* file);
private:
    FontTile* GetTile(uint32_t size);

    int m_FirstSymbol = 0;
    int m_LastSymbol = 0;

    std::unordered_map<uint32_t, std::shared_ptr<FontTile>> m_Tiles;
    unsigned char* m_FontFile = nullptr;
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
    void AddLang(int langId, std::function<LanguageTileSet()> func);
    LanguageTileSet CreateTile(int langId);
private:
    LanguageFactory();

    std::unordered_map<int, std::function<LanguageTileSet()>> m_Tiles;
};

class Font {
public:
    Font() = default;
    Font(int lang = LangId::ENG, const std::string& ttfFile = "");
    Font(const Font& other) = default;
    Font(Font&& other) = delete;

    Font& operator=(const Font& other) = default;
    Font& operator=(Font&& other) = delete;

    void GetGlyphInfo(char c, uint32_t size, GlyphInfo* info, Texture* tex) const;
private:
    bool LoadFont(const std::string& ttfFile);
private:
    std::string m_Name;
    std::unordered_map<int, std::unique_ptr<LanguageTileSet>> m_LanguageTiles;
    std::unique_ptr<unsigned char> m_TTF_File;
};


GLIB_NAMESPACE_CLOSE