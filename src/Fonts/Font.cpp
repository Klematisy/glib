#include <filesystem>

#include "Font.h"

#define vladloc(type, y) (type*) malloc(y)

glib::Font::Font(const std::string& filePath, int flags) {
    std::ifstream file(filePath, std::ios::ate);

    if (!file.is_open())
        Logger::LogErr("FONT: File '" + filePath + "' isn't open!");
    else {
        GetFontName(filePath);
        LoadFont(file, flags);
    }

    file.close();
}

void glib::Font::GetFontName(const std::string &filePath) {
    for (uint32_t i = filePath.size(); i > 0; i--) {
        if (filePath[i] == '/' || filePath[i] == '\\') {
            break;
        }
        if (filePath[i] == '.') {
            m_Name.clear();
            continue;
        }
        m_Name = filePath[i] + m_Name;
    }
}

void glib::Font::LoadFolder() const {
    std::filesystem::path folderPath = m_PathToFont;

    if (std::filesystem::exists(m_PathToFont)) {
        if (!std::filesystem::remove_all(m_PathToFont)) return;
    }

    if (std::filesystem::create_directory(folderPath))
        Logger::LogInf("FONTS: folder for fonts has created!");
    else
        return;
}

void glib::Font::LoadFont(std::ifstream &file, int flags) {
    uint32_t size = file.tellg();
    file.seekg(0);
    char* fontFromFile = (char*) (malloc(size));
    file.read(fontFromFile, size);

    m_PathToFont = "resources/Fonts/";
    m_PathToFont.append(m_Name);

    LoadFolder();

    m_PathToFont += '/';

    auto bitmap = vladloc(unsigned char, m_Width * m_Height);

    for (auto & it : LanguageCache::LangTypes) {
        if (flags & it.GetId()) {
            m_FontTileSet.emplace_back(it.GetFirstChar(), it.GetLastChar());
            LoadLangAtlas(it.GetId(), fontFromFile, bitmap);
        }
    }

    free(fontFromFile);
    free(bitmap);
}

void glib::Font::LoadLangAtlas(int id, char* fontFromFile, unsigned char* bitmap) {
    auto &it = m_FontTileSet[m_FontTileSet.size() - 1];

    it.FillFontPointer();

    stbtt_BakeFontBitmap((const unsigned char*) fontFromFile,
                         0, 400, bitmap,
                         m_Width, m_Height, it.GetFirstChar(), it.GetCount(),
                         it.GetFontPointer());

    for (uint32_t i = 0; i < it.GetCount(); i++) {
        it.GetFontPointer()[i].y0 = m_Height - it.GetFontPointer()[i].y0;
        it.GetFontPointer()[i].y1 = m_Height - it.GetFontPointer()[i].y1;
    }

    std::string pathToThisLang = m_PathToFont;

    pathToThisLang += it.GetName(id);
    pathToThisLang += ".png";
    stbi_write_png(pathToThisLang.c_str(), m_Width, m_Height, 1, bitmap, m_Width);

    it.CreateTexture(pathToThisLang.c_str());
}

const std::vector<glib::LanguageTile> &glib::Font::GetFontTileSet() const {
    return m_FontTileSet;
}
