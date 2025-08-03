#include <utility>

#include "glib.h"
#include "stb/stb_image_write.h"

#ifdef __DEBUG__
static bool write = true;
#endif

void glib::TextureManager::Bind(int slot) {
#ifdef __DEBUG__
    if (write) {
        stbi_write_png("output.png", TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, 4, m_CommonBuffer, TexInfo::WIDTH_MAX_SIZE * 4);
        write = false;
    }
#endif

    m_Texture.LoadImage(TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, m_CommonBuffer);
    m_Texture.Bind(slot);
}

void glib::TextureManager::CreateTexture() {
    for (const TexInfo &it : m_TexsInfo) {
        FillTexture(it);
    }
}

void glib::TextureManager::FillTexture(const TexInfo& it) {
    for (uint32_t i = 0; i < it.tex->GetHeight(); i++) {
        int offset = ((TexInfo::WIDTH_MAX_SIZE * (i + it.yOffset)) + it.xOffset) * 4;
        int o = (it.tex->GetWidth() * i) * 4;
        memcpy(m_CommonBuffer + offset,
               it.tex->GetBitmap() + o,
               it.tex->GetWidth() * TexInfo::BPP_MAX_LEN);
    }
}

const glib::TexInfo& glib::TextureManager::PushTexture(const Texture *t) {
    if (xPen + t->GetWidth() > TexInfo::WIDTH_MAX_SIZE) {
        xPen = 0;
        yPen += m_MaxHeight + 1;
    }

    if (yPen + t->GetHeight() > TexInfo::HEIGHT_MAX_SIZE) {
        m_DrawBuffer();
        Clear();
    }


    m_TexsInfo.push_back({t, xPen, yPen});

    xPen += t->GetWidth() + 1;
    m_MaxHeight = std::max((int)m_MaxHeight, t->GetHeight());

    return m_TexsInfo.back();
}

void glib::TextureManager::BindDrawFunc(std::function<void()> Draw) {
    m_DrawBuffer = std::move(Draw);
}

void glib::TextureManager::Clear() {
    xPen = 0;
    yPen = 0;
    m_MaxHeight = 0;
    m_TexsInfo.clear();

    std::memset(m_CommonBuffer, 0, TexInfo::BUFFER_MAX_SIZE);
}

const glib::TexInfo& glib::TextureManager::GetTexInfo(const glib::Texture *texture) {
    for (const TexInfo &it : m_TexsInfo) {
        if (texture == it.tex) {
            return it;
        }
    }

    PushTexture(texture);

    return m_TexsInfo.back();
}

glib::TextureManager::TextureManager() {
    m_CommonBuffer = (uint8_t*) std::calloc(TexInfo::BUFFER_MAX_SIZE, 1);
}

const glib::Texture &glib::TextureManager::GetBasicTex() const {
    return m_BasicTexture;
}

