#include "font_generator.h"
#include "stb/stb_image_write.h"

VLADLIB_NAMESPACE_OPEN

void FontTile::CreateAtlas(const Charset& chset, msdfgen::FontHandle* m_Font) {
    int atlasWidth = (int) (m_Size * WIDTH_K);
    int atlasHeight = (int) (m_Size * HEIGHT_K);

    FontGeometry fontGeometry(&m_FontTileInfo.glyphs);
    fontGeometry.loadCharset(m_Font, 1.0, chset);
    const double maxCornerAngle = 3.0;
    for (GlyphGeometry &glyph : m_FontTileInfo.glyphs)
        glyph.edgeColoring(&msdfgen::edgeColoringSimple, maxCornerAngle, 0);

    TightAtlasPacker packer;
    packer.setDimensionsConstraint(DimensionsConstraint::SQUARE);
    packer.setMinimumScale(m_Size);
    packer.setPixelRange(4.0);
    packer.setMiterLimit(1.0);
    packer.pack(m_FontTileInfo.glyphs.data(), (int) m_FontTileInfo.glyphs.size());

    packer.getDimensions(atlasWidth, atlasHeight);
    ImmediateAtlasGenerator<float, 3, msdfGenerator, BitmapAtlasStorage<byte, 3>> generator(atlasWidth, atlasHeight);
    GeneratorAttributes attributes;
    generator.setAttributes(attributes);
    generator.setThreadCount(4);
    generator.generate(m_FontTileInfo.glyphs.data(), (int) m_FontTileInfo.glyphs.size());

    msdfgen::BitmapConstRef<byte, 3> atlas = generator.atlasStorage();

    Range bitmap(atlasWidth * atlasHeight * 4);

    int k = 0;
    for (int i = 0; i < bitmap.r_Capacity; i+=4, k++) {
        bitmap.data[i]     = atlas.pixels[i - k];
        bitmap.data[i + 1] = atlas.pixels[i - k + 1];
        bitmap.data[i + 2] = atlas.pixels[i - k + 2];

        if (bitmap.data[i] || bitmap.data[i + 1] || bitmap.data[i + 2])
            bitmap.data[i + 3] = 255;
    }

   // stbi_write_png("atlas.png", bitmap.r_Capacity,
   //                bitmap.data(), atlasWidth * 4);

    m_FontTileInfo.tex.Init(atlasWidth, atlasHeight, 4, std::move(bitmap));
}

uint32_t FontTile::GetSize() const {
    return m_Size;
}

void FontTile::SetSize(uint32_t size) {
    m_Size = size;
}

const FontTileInfo& FontTile::GetFontTileInfo() const {
    if (m_FontTileInfo.glyphs.empty()) {
        LOGWARN("FONT TILE: Font tile is empty!");
    }
    return m_FontTileInfo;
}

VLADLIB_NAMESPACE_CLOSE
