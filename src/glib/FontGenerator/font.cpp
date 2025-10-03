#include "font.h"

#include <utility>

GLIB_NAMESPACE_OPEN

Font::Font(int lang, const std::string& ttfFile) {
    if (LoadFont(ttfFile)) {
        auto &factory = LanguageFactory::Get();
        for (auto id : LangId::langIds) {
            if ((lang & id) == 0) continue;

            m_LanguageTiles[id] = std::make_unique<LanguageTileSet>();
            *m_LanguageTiles[id] = factory.CreateTile(id);

            m_LanguageTiles[id]->SetFontFile(m_TTF_File.get());
        }
    } else {
        Logger::LogErr("FONT", "Font hasn't loaded");
    }
}

bool Font::LoadFont(const std::string &ttfFile) {
    std::ifstream file(ttfFile, std::ios::ate | std::ios::binary);

    if (!file.is_open()) return false;

    uint32_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    auto rawBitmap = (char*) std::malloc(size);
    file.read(rawBitmap, size);
    m_TTF_File = std::unique_ptr<unsigned char>((unsigned char*) rawBitmap);

    file.close();

    return true;
}

void Font::GetGlyphInfo(char c, uint32_t size, GlyphInfo* info, Texture* tex) const {
    for (auto &it : m_LanguageTiles) {
        auto &tile = *it.second;
        if (tile.GetFirstSymbol() <= c && c <= tile.GetLastSymbol()) {
            int num = c - tile.GetFirstSymbol();
            *info = tile.GetFontTileInfo(size)[num];
            *tex = *tile.GetFontTile(size);
            return;
        }
    }

    using namespace std::string_literals;
    Logger::LogWar("FONT", "GetGlyphInfo hasn't found the character: '"s + c + "'");
}

LanguageFactory::LanguageFactory() {
    AddLang(LangId::RU, std::function<LanguageTileSet()>([]() {
        LanguageTileSet languageTile(1040, 1105);
        return languageTile;
    }));

    AddLang(LangId::ENG, std::function<LanguageTileSet()>([]() {
        LanguageTileSet languageTile(32, 122);
        return languageTile;
    }));
}

LanguageFactory& LanguageFactory::Get() {
    static LanguageFactory lf;
    return lf;
}

void LanguageFactory::AddLang(int langId, std::function<LanguageTileSet()> func) {
    if (m_Tiles.find(langId) != m_Tiles.cend()) {
        Logger::LogErr("FONT", "THIS FONT ALREADY EXISTS");
        return;
    }

    m_Tiles[langId] = std::move(func);
}

LanguageTileSet LanguageFactory::CreateTile(int langId) {
    if (m_Tiles.find(langId) == m_Tiles.cend()) {
        Logger::LogErr("FONT", "FATAL ERROR WITH FONT CREATING");
        m_Tiles.begin()->second(); // LanguageFactory always not empty
    }

    return m_Tiles[langId]();
}

LanguageTileSet::LanguageTileSet(int firstSymbol, int lastSymbol)
    : m_FirstSymbol(firstSymbol), m_LastSymbol(lastSymbol)
{}

LanguageTileSet::LanguageTileSet(const LanguageTileSet &other) {
    *this = other;
}

LanguageTileSet& LanguageTileSet::operator=(const LanguageTileSet &other) {
    m_FontFile = other.m_FontFile;
    m_LastSymbol = other.m_LastSymbol;
    m_FirstSymbol = other.m_FirstSymbol;
    m_Tiles = other.m_Tiles;

    return *this;
}

int LanguageTileSet::GetFirstSymbol() const { return m_FirstSymbol; }
int LanguageTileSet::GetLastSymbol() const { return m_LastSymbol; }
void LanguageTileSet::SetFirstSymbol(int fs) { m_FirstSymbol = fs; }
void LanguageTileSet::SetLastSymbol(int ls) { m_LastSymbol = ls; }

FontTile* LanguageTileSet::GetTile(uint32_t size) {
    if (m_Tiles.find(size) == m_Tiles.cend()) {
        m_Tiles[size] = std::make_shared<FontTile>(size);
        m_Tiles[size]->CreateAtlas(m_FirstSymbol, m_LastSymbol, m_FontFile);
    }

    return m_Tiles[size].get();
}

const Glyphs& LanguageTileSet::GetFontTileInfo(uint32_t size) { return GetTile(size)->GetGlyphs(); }
const Texture* LanguageTileSet::GetFontTile(uint32_t size) { return GetTile(size)->GetTex(); }
void LanguageTileSet::SetFontFile(unsigned char *file) { m_FontFile = file; }
unsigned char* LanguageTileSet::GetFontFile() const { return m_FontFile; }

FontTile::FontTile(uint32_t size) : m_Size(size) {}

FontTile::FontTile(const FontTile& other) {
    *this = other;
}

FontTile& FontTile::operator=(const glib::FontTile &other) {
    m_Size = other.m_Size;
    m_Tex = other.m_Tex;
    m_Glyphs = other.m_Glyphs;
}

#include "stb_image_write.h"

void FontTile::CreateAtlas(int firstSymbol, int lastSymbol, unsigned char* fontFile) {
    uint32_t atlasWidth = m_Size * WIDTH_K;
    uint32_t atlasHeight = m_Size * HEIGHT_K;
    std::unique_ptr<unsigned char> oneChannelBitmap((unsigned char*) calloc(atlasWidth * atlasHeight, 1));

    stbtt_fontinfo font;
    stbtt_InitFont(&font, fontFile, stbtt_GetFontOffsetForIndex(fontFile, 0));

    int ascent, descent, lineGap;
    float scale, baseLine;

    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
    scale = stbtt_ScaleForPixelHeight(&font, (float) m_Size);
    baseLine = (float) ascent * scale;

    int penX = 0;
    int penY = 0;
    int rowHeight = 0;

    m_Glyphs.resize(lastSymbol - firstSymbol);

    for (uint32_t i = firstSymbol; i < lastSymbol; i++) {
        int advance, lsb;
        int x0, y0, x1, y1;
        int s0, t0, s1, t1;

        stbtt_GetCodepointHMetrics(&font, (int) i, &advance, &lsb);
        stbtt_GetCodepointBitmapBox(&font, (int) i, scale, scale, &x0, &y0, &x1, &y1);

        int glyphW = x1 - x0;
        int glyphH = y1 - y0;

        if (penX + glyphW >= atlasWidth) {
            penX = 0;
            penY += rowHeight + 1;
            rowHeight = 0;
        }

        stbtt_MakeCodepointBitmap(&font,
                                  oneChannelBitmap.get() + penY * atlasWidth + penX,
                                  glyphW, glyphH, (int) atlasWidth,
                                  scale, scale,
                                  (int) i);

        s0 = penX;
        s1 = penX + glyphW;
        t0 = penY;
        t1 = penY + glyphH;

        m_Glyphs[i - firstSymbol] = {glyphW, glyphH,
                                   x0, (int) baseLine + y1,
                                   advance * scale,
                                   (float) s0 / atlasWidth, (float) t0 / atlasHeight,
                                   (float) s1 / atlasWidth, (float) t1 / atlasHeight};

        penX += glyphW + 1;
        if (glyphH > rowHeight) rowHeight = glyphH;
    }

    std::shared_ptr<unsigned char> bitmap((unsigned char*) calloc(atlasWidth * atlasHeight * 4, 1));

    for (int i = 0; i < atlasWidth * atlasHeight * 4; i+=4) {
        bitmap.get()[i]     = oneChannelBitmap.get()[i / 4];
        bitmap.get()[i + 1] = 0;
        bitmap.get()[i + 2] = 0;
        bitmap.get()[i + 3] = 255;
    }

    stbi_write_png("lololol.png", (int) atlasWidth, (int) atlasHeight, 1,
                   oneChannelBitmap.get(), (int) atlasWidth);

    m_Tex = std::make_shared<Texture>(atlasWidth, atlasHeight, 4, bitmap);
}

uint32_t FontTile::GetSize() const { return m_Size; }
void FontTile::SetSize(uint32_t size) { m_Size = size; }
const Glyphs& FontTile::GetGlyphs() const { return m_Glyphs; }
const Texture* FontTile::GetTex() const { return m_Tex.get(); }

GLIB_NAMESPACE_CLOSE
