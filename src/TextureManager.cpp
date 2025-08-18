#include <utility>

#include "glib.h"
#include "stb/stb_image_write.h"
#include "graphicsUtils.h"


#ifdef __DEBUG__
static bool write = true;
#endif

glib::TextureManager::TextureManager() {
    m_CommonBuffer = (uint8_t*) std::calloc(TexInfo::BUFFER_MAX_SIZE, 1);
    m_Textures.emplace_back();
}

void glib::TextureManager::BindDrawFunc(std::function<void()> Draw) {
    m_DrawBuffer = std::move(Draw);
}

void glib::TextureManager::Bind(int slot) {
    m_Textures[slot].Bind(0);
}

void glib::TextureManager::CreateTexture(uint32_t slot) {
    FillTexture(m_TexsInfo.back());
    m_Textures[slot].LoadImage(TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, m_CommonBuffer);
}

void glib::TextureManager::FillTexture(const TexInfo& it) {
    for (uint32_t i = 0; i < it.GetTex()->GetHeight(); i++) {
        int offset = (int)((TexInfo::WIDTH_MAX_SIZE * (i + it.GetYOffset())) + it.GetXOffset()) * 4;
        int o = (int)(it.GetTex()->GetWidth() * i) * 4;
        memcpy(m_CommonBuffer + offset,
               it.GetTex()->GetBitmap() + o,
               it.GetTex()->GetWidth() * TexInfo::BPP_MAX_LEN);
    }
}

void glib::TextureManager::PushTexture(const Texture *t) {
    if (xPen + t->GetWidth() > TexInfo::WIDTH_MAX_SIZE) {
        xPen = 0;
        yPen += m_MaxHeight + 1;
    }

    if (yPen + t->GetHeight() > TexInfo::HEIGHT_MAX_SIZE) {
        Clear();
        m_Textures.emplace_back();
    }

    m_TexsInfo.emplace_back(t, xPen, yPen, (uint32_t) m_Textures.size() - 1);

    CreateTexture(m_Textures.size() - 1);

    xPen += t->GetWidth() + 1;
    m_MaxHeight = std::max((int)m_MaxHeight, t->GetHeight());
}

const glib::TexInfo& glib::TextureManager::GetTexInfo(const glib::Texture *texture) {
    for (const TexInfo &it : m_TexsInfo) {
        if (texture == it.GetTex()) {
            return it;
        }
    }

    PushTexture(texture);

    return m_TexsInfo.back();
}

void glib::TextureManager::Clear() {
    xPen = 0;
    yPen = 0;
    m_MaxHeight = 0;

    std::memset(m_CommonBuffer, 0, TexInfo::BUFFER_MAX_SIZE);
}

const glib::Texture &glib::TextureManager::GetBasicTex() const {
    return m_BasicTexture;
}

#ifdef __DEBUG__
void glib::TextureManager::PrintTextures() {
    std::string name = "output";
    name.append(std::to_string(m_Textures.size()));
    name.append(".png");
    if (write) {
        stbi_write_png(name.c_str(), TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, 4, m_CommonBuffer, TexInfo::WIDTH_MAX_SIZE * 4);
        write = false;
    }
}
#endif
