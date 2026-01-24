#include <utility>
#include <mutex>

#include "stb/stb_image_write.h"
#include "texture_atlas.h"

template<class T>
using vec = std::vector<T>;

template<class KEY, class VALUE>
using map = std::unordered_map<KEY, VALUE>;

GLIB_NAMESPACE_OPEN


bool Slot::PushBack(const rc::ImageInfo* texture) {
    if (m_Pen.x + texture->GetWidth() > m_W) {
        m_Pen.x = 0;
        m_Pen.y += m_MaxRowH;
        m_MaxRowH = 0;
    }

    if (m_Pen.y + texture->GetWidth() > m_H) {
        return false;
    }

    TexInfoPtr info = std::make_shared<TexInfo>();

    Rectangle imageInfo {
        (float) m_Pen.x              / (float) m_W,
        (float) m_Pen.y              / (float) m_H,
        (float) texture->GetWidth()  / (float) m_W,
        (float) texture->GetHeight() / (float) m_H
    };

    m_Pen.x += texture->GetWidth() + 1;
    m_MaxRowH = std::max(m_MaxRowH, (uint32_t) texture->GetHeight());

    info->SetImageRectangle(imageInfo);
    m_Textures[texture] = info;

    return true;
}

TexInfoPtr Slot::GetTexInfo(const rc::ImageInfo* texture) const {
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



TextureAtlas::TextureAtlas(std::shared_ptr<RendererCore::TextureArray> textureObject)
        : m_TextureObject(std::move(textureObject))
{
    m_Slots.resize(m_TextureObject->GetLayersCount());
    for (auto& it : m_Slots)
        it.SetSlotSize(m_TextureObject->GetWidth(), m_TextureObject->GetHeight());
}

TexInfoConstRef TextureAtlas::GetTexInfo(const rc::ImageInfo* texture) {
    if (!texture->GetBitmap()) {
        Logger::LogErr("Texture Atlas", "The texture doesn't exist!");
        return m_SingleTexture;
    }

    for (uint32_t i = 1; i < m_Slots.size(); i++) {
        auto& slot = m_Slots[i];

        if (auto info = slot.GetTexInfo(texture)) {
            return info;
        } else if (slot.PushBack(texture)) {
            info = slot.GetTexInfo(texture);
            info->SetSlot(i);

            const Rectangle& imInf = info->GetRectangle();

            auto* texArr = (RendererCore::TextureArray*) m_TextureObject.get();

            float w = (float) texArr->GetWidth();
            float h = (float) texArr->GetHeight();

            texArr->AddImage(*texture,
                  (uint32_t) (imInf.x * w),
                  (uint32_t) (imInf.y * h), i);

            return info;
        }
    }

    return m_SingleTexture;
}

const RendererCore::TextureArray* TextureAtlas::GetTextureObject() const {
    return m_TextureObject.get();
}

GLIB_NAMESPACE_CLOSE