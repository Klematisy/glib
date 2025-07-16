#pragma once

#include <fstream>
#include <iostream>
#include <memory>

#include "stb/stb_image_write.h"
#include "stb/stb_truetype.h"
#include "../OpenGLCore/Logger.h"

#include "../Window.h"
#include "../OpenGLCore/Renderer.h"

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
        LanguageData() = default;
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
        LanguageTile(uint32_t size, uint32_t count, int firstElement);
        void CreateAtlas(const unsigned char *font, wchar_t firstChar);
        void GetSymbolQuad(float *xPos, float *yPos, wchar_t symbol, stbtt_aligned_quad *quad);

        int GetSize() const;
        int GetWidth() const;
        int GetHeight() const;
        stbtt_bakedchar* GetFontPointer() const;
        const GlCore::Texture& GetTexture() const;

    private:
        void FillFontPointer(uint32_t count);
    private:
        static constexpr uint32_t s_WidthCoefficient  = 10;
        static constexpr uint32_t s_HeightCoefficient = 6;

        int m_Size = 0;
        int m_AtlasWidth = 0;
        int m_AtlasHeight = 0;
        int m_Count = 0;
        int m_FirstElementIndex = 0;
        std::unique_ptr<GlCore::Texture> m_FontAtlas;
        std::unique_ptr<stbtt_bakedchar[]> m_TileCoordinates;
    };

    class LanguageTileSet : public LangRange {
    public:
        LanguageTileSet(std::unique_ptr<char[]> *fontFile, wchar_t firstChar, wchar_t lastChar);
        LanguageTileSet(LanguageTileSet&& other) noexcept;

        LanguageTile& GetTile(uint32_t size);
    private:
        std::unique_ptr<char[]> *m_FontFile = nullptr;
        std::vector<LanguageTile> m_LanguageTiles;
    };

    class Language {
    public:
        static const int ENG = 1 << 0; // 0001
        static const int RU  = 1 << 1; // 0010
    };

    namespace LanguageCache {
        static constexpr const char* FontPath = "resources/Fonts/";

        static constexpr uint32_t count = 2;
        static const LanguageData LangTypes[count] {
                LanguageData(Language::ENG, L' ', L'}'),
                LanguageData(Language::RU,  L'А', L'я')
        };
    };

    class Font {
    public:
        Font(const std::string& filePath, int flags = Language::ENG);

        std::vector<LanguageTileSet>& GetFontTileSet();
    private:
        void LoadFont(std::ifstream &file, int flags);

        std::unique_ptr<char[]> m_FontFile;
        std::vector<LanguageTileSet> m_FontTileSet;
    };
}
