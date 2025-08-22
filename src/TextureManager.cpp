#include <utility>

#include "glib.h"
#include "stb/stb_image_write.h"
#include "graphicsUtils.h"

template<class... Args>
void glib::Slot::EmplaceBack(Args&&... args) {
    m_SlotInfo.emplace_back(std::forward<Args>(args)...);
}

const glib::Slot::InfoArr &glib::Slot::GetInfo() const {
    return m_SlotInfo;
}





glib::TextureManager::TextureManager() {
    auto temp = (uint8_t*) std::calloc(TexInfo::BUFFER_MAX_SIZE, 1);
    m_CommonBuffer = std::unique_ptr<uint8_t>(temp);

    m_Textures = GlCore::TextureArray(TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, LAYERS);
}

void glib::TextureManager::Bind() {
    m_Textures.Bind();
}

void glib::TextureManager::CreateTexture(uint32_t slot) {
    FillTexture(m_TexsInfo[m_FilledSlots].GetInfo().back());
    m_Textures.LoadImage((char*)m_CommonBuffer.get(), slot);
}

void glib::TextureManager::FillTexture(const TexInfo& it) {
    uint8_t* tmp = m_CommonBuffer.get();
    for (uint32_t i = 0; i < it.GetTex()->GetHeight(); i++) {
        int offset1 = (int)((TexInfo::WIDTH_MAX_SIZE * (i + it.GetYOffset())) + it.GetXOffset()) * 4;
        int offset2 = (int)(it.GetTex()->GetWidth() * i) * 4;
        memcpy(tmp + offset1,
               it.GetTex()->GetBitmap() + offset2,
               it.GetTex()->GetWidth() * TexInfo::BPP_MAX_LEN);
    }
}

const glib::TexInfo& glib::TextureManager::PushTexture(const Texture *t) {
    if (xPen + t->GetWidth() > TexInfo::WIDTH_MAX_SIZE) {
        xPen = 0;
        yPen += m_MaxHeight + 1;
    }

    if (yPen + t->GetHeight() > TexInfo::HEIGHT_MAX_SIZE) {
#ifdef __GLIB_DEBUG__
        PrintTextures();
#endif
        m_Textures.LoadImage((char*)m_CommonBuffer.get(), m_FilledSlots++);
        Clear();
    }

    auto &it = m_TexsInfo[m_FilledSlots];
    it.EmplaceBack(t, xPen, yPen, m_FilledSlots);

    CreateTexture(m_FilledSlots);

    xPen += t->GetWidth() + 1;
    m_MaxHeight = std::max((int)m_MaxHeight, t->GetHeight());

    return it.GetInfo().back();
}

const glib::TexInfo& glib::TextureManager::GetTexInfo(const glib::Texture *texture) {
    for (const Slot &it : m_TexsInfo) {
        for (const TexInfo& info : it.GetInfo()) {
            if (texture == info.GetTex()) {
                return info;
            }
        }
    }

    return PushTexture(texture);
}

void glib::TextureManager::Clear() {
    xPen = 0;
    yPen = 0;
    m_MaxHeight = 0;

    std::memset(m_CommonBuffer.get(), 0, TexInfo::BUFFER_MAX_SIZE);
}

const glib::Texture &glib::TextureManager::GetBasicTex() const {
    return m_BasicTexture;
}

#ifdef __GLIB_DEBUG__
void glib::TextureManager::PrintTextures() {
    std::string name = "output";
    name.append(std::to_string(m_FilledSlots));
    name.append(".png");
    stbi_write_png(name.c_str(), TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, 4, m_CommonBuffer.get(), TexInfo::WIDTH_MAX_SIZE * 4);
}
#endif
