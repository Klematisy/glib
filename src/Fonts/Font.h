#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <array>

#include "stb/stb_image_write.h"
#include "stb/stb_truetype.h"
#include "../OpenGLCore/Logger.h"

#include "../window.h"

#include "../graphicsUtils.h"

namespace glib {

    class LangRange {
    public:
        LangRange(wchar_t firstChar, wchar_t lastChar);
        wchar_t GetFirstChar() const;
        wchar_t GetLastChar() const;
        int GetCount() const;

    protected:
        wchar_t m_FirstCharacter = L' ';
        wchar_t m_LastCharacter  = L' ';
    };

    class LanguageData : public LangRange {
    public:
        LanguageData& operator=(const LanguageData& other);
        LanguageData(int id, wchar_t firstChar, wchar_t lastChar);
        LanguageData(const LanguageData& other) noexcept;

        static const char* GetName(int id);
        int GetId() const;

    private:
        int m_Id = 0;
    };

    class LanguageTile {
    public:
        LanguageTile() = default;
        LanguageTile(uint32_t size, const LangRange *langRange);
        void CreateAtlas(std::shared_ptr<unsigned char> &fontFile);
        void GetSymbolQuad(float *x, float y, wchar_t symbol, stbtt_aligned_quad *quad);

        int GetSize() const;
        const class Texture* GetTexture() const;

    private:
        struct GlyphInfo {
            int width, height;
            int xOffset, yOffset;
            float advance;
            float s0, t0, s1, t1;
        };
    private:
        void FillFontPointer(uint32_t count);
    private:
        static constexpr uint32_t s_WidthCoefficient  = 10;
        static constexpr uint32_t s_HeightCoefficient = 6;

        int m_Size = 0;
        int m_AtlasWidth = 0, m_AtlasHeight = 0;
        const LangRange *m_LangRange = nullptr;

        std::unique_ptr<GlyphInfo[]> m_Glyphs;
        std::unique_ptr<Texture> m_FontAtlas;
    };

    class LanguageTileSet : public LangRange {
    public:
        LanguageTileSet(std::shared_ptr<unsigned char> fontFile, wchar_t firstChar, wchar_t lastChar);
        LanguageTileSet(LanguageTileSet&& other) noexcept;

        LanguageTile& GetTile(uint32_t size) const;
    private:
        mutable std::shared_ptr<unsigned char> m_FontFile;
        mutable std::vector<LanguageTile> m_LanguageTiles;
    };

    class Font;

    class Language {
    public:
        static constexpr int ENG = 1 << 0; // 0001
        static constexpr int RU  = 1 << 1; // 0010

        static constexpr int ALL = ENG | RU;
    };

    class Font {
    public:
        Font() = default;
        Font(const std::string& filePath, int flags = Language::ENG);

        Font& operator=(Font&& other);

        const std::vector<LanguageTileSet>& GetFontTileSet() const;
    private:
        void LoadFont(std::ifstream &file, int flags);

        std::shared_ptr<void> m_FontFile;
        std::vector<LanguageTileSet> m_FontTileSet;
    };

    class LangFontCache {
    public:
        static constexpr const char* fontPath = "resources/Fonts/";
        static constexpr const char* stdFont  = "Helvetica.ttf";

        static constexpr uint32_t count = 2;

        static LangFontCache& GetCache() { return s_FontCacheInstance; }

        const std::array<LanguageData, count>& GetLangTypes() const;
        const Font* GetBasicFont() const;

        void LoadFontCache();
    private:
        const std::array<LanguageData, count> m_LangTypes {
            LanguageData(Language::ENG, L' ', L'}'),
            LanguageData(Language::RU,  L'А', L'ё')
        };
        Font m_BasicFont = Font();

        LangFontCache() { LoadFontCache(); }
        static LangFontCache s_FontCacheInstance;
    };

    inline LangFontCache LangFontCache::s_FontCacheInstance;
}
