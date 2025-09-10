#include <utility>
#include <mutex>

#include "glib.h"
#include "stb/stb_image_write.h"
#include "graphicsUtils.h"

template<class T>
using vec = std::vector<T>;

template<class KEY, class VALUE>
using map = std::unordered_map<KEY, VALUE>;

glib::Slot::Slot() {
    m_CommonBuffer = std::unique_ptr<uint8_t>((uint8_t*)std::calloc(TexInfo::BUFFER_MAX_SIZE, 1));
}

void glib::Slot::Sort(uint32_t key) {
    auto &unsortedRow = m_Rows[key].images;

    for (uint32_t i = 0; i < unsortedRow.size(); i++) {
        bool sorted = true;
        for (uint32_t j = 0; j < unsortedRow.size() - 1; j++) {
            if (unsortedRow[j].GetTex()->GetHeight() < unsortedRow[j + 1].GetTex()->GetHeight()) {
                TexInfo temp  = unsortedRow[j];
                unsortedRow[j] = unsortedRow[j + 1];
                unsortedRow[j + 1] = temp;
                sorted = false;
            }
        }
        if (sorted) break;
    }

    uint32_t w = 0;
    for (auto &it : unsortedRow) {
//        if (it.GetTex()->GetWidth() == 1 && it.GetTex()->GetHeight() == 1) {
//            it.SetXOffset(0);
//            it.SetYOffset(1);
//            continue;
//        }
        it.SetXOffset(w);
        w += it.GetTex()->GetWidth();
        w += 1;
    }
}

void glib::Slot::Cut(uint32_t key) {
    const auto &sortedRow = m_Rows[key];

    for (const auto &image : sortedRow.images) {
        glm::vec2 extremumPoint(image.GetXOffset(),
                                image.GetYOffset() + image.GetTex()->GetHeight());

        m_FreeRects.push_back({extremumPoint.x, extremumPoint.y,
                               (float)(TexInfo::WIDTH_MAX_SIZE - extremumPoint.x),
                               (float)(sortedRow.maxHeight - image.GetTex()->GetHeight())
        });
    }
}

bool glib::Slot::FindFreeSpace(const TexInfo& tex) {
    auto &t = *tex.GetTex();

    for (uint32_t i = 0; i < m_FreeRects.size(); i++) {
        if (tex.GetTex()->GetWidth() <= m_FreeRects[i].width &&
            tex.GetTex()->GetHeight() <= m_FreeRects[i].height) {

            auto &imgs = m_Rows[(uint32_t)m_FreeRects[i].y].images;
            imgs.emplace_back(tex.GetTex(), (uint32_t)m_FreeRects[i].x, (uint32_t)m_FreeRects[i].y, tex.GetSlot());
            FillImage(imgs.back());

            Cut((uint32_t)m_FreeRects[i].y);
            m_FreeRects.erase(m_FreeRects.cbegin() + i);
            return true;
        }
    }

    return false;
}

void glib::Slot::FillImage(const TexInfo& info) {
    uint8_t* tmp = m_CommonBuffer.get();
    for (uint32_t i = 0; i < info.GetTex()->GetHeight(); i++) {
        int offset1 = (int)((TexInfo::WIDTH_MAX_SIZE * (i + info.GetYOffset())) + info.GetXOffset()) * 4;
        int offset2 = (int)(info.GetTex()->GetWidth() * i) * 4;
        memcpy(tmp + offset1,
               info.GetTex()->GetBitmap() + offset2,
               info.GetTex()->GetWidth() * TexInfo::BPP_MAX_LEN);
    }
}

void glib::Slot::FillRow(uint32_t key) {
    auto &row = m_Rows[key];
    std::memset(m_CommonBuffer.get() + key * TexInfo::BPP_MAX_LEN * TexInfo::WIDTH_MAX_SIZE, 0, m_MaxHeight * TexInfo::BPP_MAX_LEN * TexInfo::WIDTH_MAX_SIZE);

    for (auto &info : row.images) {
        FillImage(info);
    }
}

bool glib::Slot::PushBack(const TexInfo& info) {
    if (m_XPen + info.GetTex()->GetWidth() > TexInfo::WIDTH_MAX_SIZE) {
        m_Rows[m_YPen].maxHeight = m_MaxHeight;

        Sort(m_YPen);
        FillRow(m_YPen);
//        Cut(m_YPen);

        m_XPen = 0;
        m_YPen += m_MaxHeight + 1;
        m_MaxHeight = 0;
    }

    if (m_YPen + info.GetTex()->GetHeight() > TexInfo::HEIGHT_MAX_SIZE) {

//        return FindFreeSpace(info);
        return false;
    }

    m_Rows[m_YPen].images.emplace_back(info.GetTex(), m_XPen, m_YPen, info.GetSlot());
    FillImage(m_Rows[m_YPen].images.back());

    m_XPen += info.GetTex()->GetWidth() + 1;
    m_MaxHeight = std::max((int)m_MaxHeight, info.GetTex()->GetHeight());

    return true;
}

const uint8_t* glib::Slot::GetData() const {
    return m_CommonBuffer.get();
}

const map<uint32_t, glib::Slot::Row> &glib::Slot::GetInfo() const {
    return m_Rows;
}








glib::TextureManager::TextureManager() {
    m_Textures = GlCore::TextureArray(TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, LAYERS);
}

void glib::TextureManager::Bind() {
    m_Textures.Bind();
}

const glib::TexInfo& glib::TextureManager::PushTexture(const Texture *t) {
    for (uint32_t i = FIRST_SLOT; i < LAYERS; i++) {
//        if (m_TexsInfo[i].GetInfo().size() == 0) break;

        m_LastCreatedEl = {t, 0, 0, i};

        if (m_TexsInfo[i].PushBack(m_LastCreatedEl)) {
            m_Textures.LoadImage((char*)m_TexsInfo[i].GetData(), i);
            break;
        }
    }

    return m_LastCreatedEl;
}

const glib::TexInfo& glib::TextureManager::GetTexInfo(const glib::Texture *texture) {
    for (uint32_t i = FIRST_SLOT; i < LAYERS; i++) {
        auto &it = m_TexsInfo[i];
        for (auto &row : it.GetInfo()) {
            for (auto &info : row.second.images) {
                if (texture == info.GetTex()) return info;
            }
        }
    }

    return PushTexture(texture);
}


const glib::Texture &glib::TextureManager::GetBasicTex() const {
    return m_BasicTexture;
}

#ifdef __GLIB_DEBUG__
void glib::TextureManager::PrintTextures(int i) {
    std::string name = "output";
    name.append(std::to_string(i));
    name.append(".png");
    stbi_write_png(name.c_str(), TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, 4,
                   m_TexsInfo[i].GetData(), TexInfo::WIDTH_MAX_SIZE * 4);
}
#endif
