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

    class LanguageData {
    public:
        LanguageData() = default;
        LanguageData& operator=(const LanguageData& other);
        LanguageData(int id, wchar_t firstChar, wchar_t lastChar);
        LanguageData(const LanguageData& other) noexcept;

        const char* GetName(int id);
        int GetId() const;
        wchar_t GetFirstChar() const;
        wchar_t GetLastChar() const;
        int GetCount() const;

    private:
        int m_Id = 0;
    protected:
        wchar_t m_FirstCharacter = L' ';
        wchar_t m_LastCharacter  = L' ';
    };

    class LanguageTile : public LanguageData {
    public:
        LanguageTile() = default;
        LanguageTile(LanguageTile&& other) noexcept;
        LanguageTile(wchar_t firstChar, wchar_t lastChar);

        void FillFontPointer();
        void CreateTexture(const char* imagePath);
        const GlCore::Texture& GetTexture() const;
        stbtt_bakedchar* GetFontPointer() const;
    private:
        std::unique_ptr<stbtt_bakedchar[]> m_FontTileCoordinates;
        std::unique_ptr<GlCore::Texture> m_FontAtlas;
    };

    class Language {
    public:
        static const int ENG = 1 << 0; // 0001
        static const int RU  = 1 << 1; // 0010
    };

    namespace LanguageCache {
        static constexpr uint32_t count = 2;
        static const LanguageData LangTypes[count] {
            LanguageData(Language::ENG, L' ', L'}'),
            LanguageData(Language::RU,  L'А', L'я')
        };
    };


    class Font {
    public:
        Font(const std::string& filePath, int flags = Language::ENG);

        const std::vector<LanguageTile>& GetFontTileSet() const;
    private:
        static constexpr int m_Width  = 3840;
        static constexpr int m_Height = 2160;

        void GetFontName(const std::string& filePath);
        void LoadFont(std::ifstream &file, int flags);
        void LoadFolder() const;
        void LoadLangAtlas(int id, char* fontFromFile, unsigned char* bitmap);

        std::string m_Name;
        std::string m_PathToFont;

        std::vector<LanguageTile> m_FontTileSet;
    };
}
