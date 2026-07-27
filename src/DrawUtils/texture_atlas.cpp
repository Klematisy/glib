#include <utility>
#include <mutex>

#include "stb/stb_image_write.h"
#include "texture_atlas.h"

template<class T>
using vec = std::vector<T>;

template<class KEY, class VALUE>
using map = std::unordered_map<KEY, VALUE>;

VLADLIB_NAMESPACE_OPEN

bool Slot::PushBack(const GAPI::ImageInfo* texture) {
    if (m_Pen.x + texture->r_Width > m_W) {
        m_Pen.x = 0;
        m_Pen.y += m_MaxRowH;
        m_MaxRowH = 0;
    }

    if (m_Pen.y + texture->r_Width > m_H) {
        return false;
    }

    TexInfoPtr info = std::make_shared<TexInfo>();

    Rectangle atlasBounds {
        (float)m_Pen.x           / (float) m_W,
        (float)m_Pen.y           / (float) m_H,
        (float)texture->r_Width  / (float) m_W,
        (float)texture->r_Height / (float) m_H,
    };

    m_Pen.x += texture->r_Width + 1;
    m_MaxRowH = std::max(m_MaxRowH, (uint32_t) texture->r_Height);

    info->atlasBounds = atlasBounds;

    m_Textures[texture] = info;

    return true;
}

TexInfoPtr Slot::GetTexInfo(const GAPI::ImageInfo* texture) const {
    const auto& iter = m_Textures.find(texture);

    if (iter == m_Textures.cend()) return nullptr;

    return iter->second;
}

void Slot::SetSlotSize(uint32_t w, uint32_t h) {
    m_W = w;
    m_H = h;
}

uint32_t Slot::GetSlotWidth() const { return m_W; }
uint32_t Slot::GetSlotHeight() const { return m_H; }





TextureAtlas::TextureAtlas(std::shared_ptr<GAPI::TextureArray> textureObject)
        : m_TextureObject(std::move(textureObject))
{
    m_Slots.resize(m_TextureObject->r_Layers);
    for (auto& it : m_Slots)
        it.SetSlotSize(m_TextureObject->r_Width, m_TextureObject->r_Height);
}

TexInfoConstRef TextureAtlas::GetTexInfo(const GAPI::ImageInfo* texture) {
//    if (!texture->GetBitmap()) { //TODO: подумать над этим
//        Logger::LogErr("Texture Atlas", "The texture doesn't exist!");
//        return m_SingleTexture;
//    }

    for (uint32_t i = 1; i < m_Slots.size(); i++) {
        auto& slot = m_Slots[i];

        if (auto info = slot.GetTexInfo(texture)) {
            return &info;
        } else if (slot.PushBack(texture)) {
            info = slot.GetTexInfo(texture);
            info->atlasSlot = i;

            const Rectangle& ab = info->atlasBounds;

            auto* texArr = (GAPI::TextureArray*) m_TextureObject.get();

            float w = (float) texArr->r_Width;
            float h = (float) texArr->r_Height;

            uint32_t x = std::round(ab.x * w);
            uint32_t y = std::round(ab.y * h);

            if (texture->r_Bitmap) {
                texArr->AddImage(*texture, x, y, i);
            }

            return &info;
        }
    }

    return &m_SingleTexture;
}

const GAPI::TextureArray* TextureAtlas::GetTextureObject() const {
    return m_TextureObject.get();
}

VLADLIB_NAMESPACE_CLOSE
