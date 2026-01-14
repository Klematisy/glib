#include <utility>
#include <mutex>

#include "stb/stb_image_write.h"
#include "texture_manager.h"

template<class T>
using vec = std::vector<T>;

template<class KEY, class VALUE>
using map = std::unordered_map<KEY, VALUE>;

GLIB_NAMESPACE_OPEN


bool Slot::PushBack(const Tex2d* texture) {
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

TexInfoPtr Slot::GetTexInfo(const Tex2d* texture) const {
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


TexInfoConstRef TextureManager::GetTexInfo(const Tex2d* texture) {
    if (m_Slots.empty()) {
        auto* tex = (RendererCore::Texture2D*) m_TextureObject.get();
        tex->Upload(*texture);

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
}

TextureManager::TextureManager(std::shared_ptr<RendererCore::ITexture>& textureObject)
    : m_TextureObject(textureObject)
{
    if (auto texArr = dynamic_cast<RendererCore::TextureArray*>(textureObject.get())) {
        m_Slots.resize(texArr->GetLayersCount());
        for (auto& it : m_Slots)
            it.SetSlotSize(texArr->GetWidth(), texArr->GetHeight());
    } else {
        m_SingleTexture->SetImageRectangle({
            0, 0, 1, 1
        });
    }
}

const RendererCore::ITexture* TextureManager::GetTextureObject() const {
    return m_TextureObject.get();
}

void TextureManager::Bind() const {
    m_TextureObject->Bind();
}

GLIB_NAMESPACE_CLOSE