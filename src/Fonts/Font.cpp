#include <filesystem>

#include "Font.h"

#define vladloc(type, y) (type*) malloc(y)

glib::Font::Font(const std::string& filePath, int flags) {
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        Logger::LogErr("FONT: File '" + filePath + "' isn't open!");
    else {
        LoadFont(file, flags);
    }

    file.close();
}

void glib::Font::LoadFont(std::ifstream &file, int flags) {
    uint32_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    m_FontFile = std::make_unique<char[]>(size);

    if (!file.read(m_FontFile.get(), size)) {
        Logger::LogErr("BLYAT'");
    }

    for (auto & it : LanguageCache::LangTypes) {
        if (flags & it.GetId()) {
            m_FontTileSet.emplace_back(&m_FontFile, it.GetFirstChar(), it.GetLastChar());
        }
    }
}

std::vector<glib::LanguageTileSet> &glib::Font::GetFontTileSet() {
    return m_FontTileSet;
}
