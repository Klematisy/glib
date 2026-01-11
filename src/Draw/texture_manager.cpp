#include <utility>
#include <mutex>

#include "stb/stb_image_write.h"
#include "texture_manager.h"
#include "texture.h"

template<class T>
using vec = std::vector<T>;

template<class KEY, class VALUE>
using map = std::unordered_map<KEY, VALUE>;

GLIB_NAMESPACE_OPEN


bool Slot::PushBack(const Texture* texture) {
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
        (float) m_Pen.x,
        (float) m_Pen.y,
        (float) texture->GetWidth(),
        (float) texture->GetHeight()
    };

    m_Pen.x += texture->GetWidth() + 1;
    m_MaxRowH = std::max(m_MaxRowH, (uint32_t) texture->GetHeight());

    info->SetImageRectangle(imageInfo);
    m_Textures[texture] = info;

    return true;
}

TexInfoPtr Slot::GetTexInfo(const Texture* texture) const {
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


TexInfoConstRef TextureManager::GetTexInfo(const Texture* texture) {
    for (uint32_t i = 0; i < m_Slots.size(); i++) {
        auto& slot = m_Slots[i];

        if (auto info = slot.GetTexInfo(texture)) {
            return info;
        } else if (slot.PushBack(texture)) {
            info = slot.GetTexInfo(texture);
            info->SetSlot(i);

            const Rectangle& imInf = info->GetRectangle();

            m_GPU_TexArr->LoadImage(texture->GetBitmap(), i,
                  (uint32_t) imInf.x, (uint32_t) imInf.y,
                  (uint32_t) imInf.width, (uint32_t) imInf.height);

            return info;
        }
    }
}

TextureManager::TextureManager(std::shared_ptr<RendererCore::TextureArray>& texArr)
    : m_GPU_TexArr(texArr)
{
    m_Slots.resize(texArr->GetLayersCount());
    for (auto& it : m_Slots)
        it.SetSlotSize(texArr->GetWidth(), texArr->GetHeight());
}

const RendererCore::TextureArray& TextureManager::GetTexArr() const {
    return *m_GPU_TexArr;
}

void TextureManager::Bind() const {
    m_GPU_TexArr->Bind();
}

GLIB_NAMESPACE_CLOSE