#include <filesystem>

#include "font.h"

glib::Font::Font(const std::string& filePath, int flags) {
    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        Logger::LogErr("FONT", "File '" + filePath + "' isn't open!");
    else {
        LoadFont(file, flags);
    }

    file.close();
}

glib::Font &glib::Font::operator=(glib::Font&& other) {
    m_FontFile = std::move(other.m_FontFile);
    m_FontTileSet = std::move(other.m_FontTileSet);

    return *this;
}

void glib::Font::LoadFont(std::ifstream &file, int flags) {
    uint32_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    m_FontFile = std::move(std::shared_ptr<unsigned char>(new unsigned char[size], std::default_delete<unsigned char>()));

    if (!file.read(std::static_pointer_cast<char>(m_FontFile).get(), size)) {
        Logger::LogErr("FONT", "File hasn't read!");
    }

    for (auto & it : LangFontCache::GetCache().GetLangTypes()) {
        if (flags & it.GetId()) {
            m_FontTileSet.emplace_back(std::static_pointer_cast<unsigned char>(m_FontFile), it.GetFirstChar(), it.GetLastChar());
        }
    }
}

const std::vector<glib::LanguageTileSet> &glib::Font::GetFontTileSet() const {
    return m_FontTileSet;
}
