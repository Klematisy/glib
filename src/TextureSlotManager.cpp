#include <utility>

#include "glib.h"
#include "stb/stb_image_write.h"

static bool write = true;

void glib::TextureSlotManager::BindDrawFunc(std::function<void()> Draw) {
    m_DrawBuffer = std::move(Draw);
}

void glib::TextureSlotManager::Bind(int slot) {
    if (write) {
        stbi_write_png("output.png", TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, 4, m_CommonBuffer, TexInfo::WIDTH_MAX_SIZE * 4);
        write = false;
    }
    m_Texture.LoadImage(TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, m_CommonBuffer);
    m_Texture.Bind(slot);
}

void glib::TextureSlotManager::FillTexture(const TexInfo& it) {
    for (uint32_t i = 0; i < it.tex->GetHeight(); i++) {
        int offset = ((TexInfo::WIDTH_MAX_SIZE * (i + it.yOffset)) + it.xOffset) * 4;
        int o = (it.tex->GetWidth() * i) * 4;
        memcpy(m_CommonBuffer + offset,
               it.tex->GetBitmap() + o,
               it.tex->GetWidth() * TexInfo::BPP_MAX_LEN);
    }
}

const glib::TexInfo& glib::TextureSlotManager::PushTexture(const Texture *t) {
    if (xPen + t->GetWidth() > TexInfo::WIDTH_MAX_SIZE) {
        xPen = 0;
        yPen += m_MaxHeight;
    }

    if (yPen + t->GetHeight() > TexInfo::HEIGHT_MAX_SIZE) {
        Bind(0);
        m_DrawBuffer();
        Clear();
    }

    m_CommonSize += t->GetSize();
    m_TexsInfo.push_back({t, xPen, yPen});
    FillTexture(m_TexsInfo.back());

    xPen += t->GetWidth();
    m_MaxHeight = std::max((int)m_MaxHeight, t->GetHeight());


    return m_TexsInfo.back();
}

void glib::TextureSlotManager::Clear() {
    xPen = 0;
    yPen = 0;
    m_MaxHeight = 0;
    m_CommonSize = 0;

    std::free(m_CommonBuffer);
    m_CommonBuffer = (uint8_t*) std::calloc(TexInfo::BUFFER_MAX_SIZE, 1);
}

const glib::TexInfo& glib::TextureSlotManager::GetTexInfo(const glib::Texture *texture) {
    for (const TexInfo &it : m_TexsInfo) {
        if (texture == it.tex) {
            if (m_CommonSize + it.tex->GetSize() >= TexInfo::BUFFER_MAX_SIZE) {
                Bind(0);
                m_DrawBuffer();
                Clear();
            }

            m_CommonSize += it.tex->GetSize();

            FillTexture(it);
            return it;
        }
    }

    Logger::LogWar("Texture hasn't found! GetTexInfo will create new texture...");
    PushTexture(texture);

    return m_TexsInfo.back();
}

glib::TextureSlotManager::TextureSlotManager() {
    m_CommonBuffer = (uint8_t*) std::calloc(TexInfo::BUFFER_MAX_SIZE, 1);
}

const glib::Texture &glib::TextureSlotManager::GetBasicTex() const {
    return m_BasicTexture;
}

