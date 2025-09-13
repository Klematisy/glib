#include <utility>
#include <mutex>

#include "glib.h"
#include "stb/stb_image_write.h"
#include "texture_manager.h"
#include "texture.h"

template<class T>
using vec = std::vector<T>;

template<class KEY, class VALUE>
using map = std::unordered_map<KEY, VALUE>;

GLIB_NAMESPACE_OPEN

Slot::Slot() {
#ifdef __GLIB_DEBUG__
//    m_CommonBuffer = std::unique_ptr<uint8_t>((uint8_t*)std::calloc(TexInfo::BUFFER_MAX_SIZE, 1));
#endif
}

void Slot::Sort(uint32_t key) {
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
        it.SetXOffset(w);
        w += it.GetTex()->GetWidth();
        w += 1;
    }
}

void Slot::Cut(uint32_t key) {
    const auto &row = m_Rows[key];
    const auto &sortedRow = row.images;

    for (uint32_t i = 1; i < sortedRow.size(); i++) {
        const auto &PastImage = sortedRow[i - 1];
        const auto &NowImage = sortedRow[i];

        if (NowImage.GetYOffset() == row.maxHeight) continue;

        glm::vec2 extremumPoint(NowImage.GetXOffset(),
                                NowImage.GetYOffset() + NowImage.GetTex()->GetHeight());

        m_FreeRects.push_back({extremumPoint.x, extremumPoint.y,
                               (float)(TexInfo::WIDTH_MAX_SIZE - extremumPoint.x),
                               (float)(PastImage.GetTex()->GetHeight() - NowImage.GetTex()->GetHeight())
        });
    }

    auto &lastEl = sortedRow.back();

    glm::vec2 extremumPoint(lastEl.GetXOffset() + lastEl.GetTex()->GetWidth(),
                            lastEl.GetYOffset());

    if (extremumPoint.x == TexInfo::WIDTH_MAX_SIZE) return;

    m_FreeRects.push_back({extremumPoint.x, extremumPoint.y,
                           TexInfo::WIDTH_MAX_SIZE - extremumPoint.x,
                           (float)lastEl.GetTex()->GetHeight()});
}

const TexInfo* glib::Slot::FindFreeSpace(const TexInfo& tex) {
    auto &t = *tex.GetTex();

    for (uint32_t i = 0; i < m_FreeRects.size(); i++) {
        if (tex.GetTex()->GetWidth() <= m_FreeRects[i].width &&
            tex.GetTex()->GetHeight() <= m_FreeRects[i].height) {

            auto &imgs = m_Rows[(uint32_t)m_FreeRects[i].y].images;
            imgs.emplace_back(tex.GetTex(), (uint32_t)m_FreeRects[i].x, (uint32_t)m_FreeRects[i].y, tex.GetSlot());
//            FillImage(imgs.back());

            Cut((uint32_t)m_FreeRects[i].y);
            m_FreeRects.erase(m_FreeRects.cbegin() + i);

            Sort((uint32_t) m_FreeRects[i].y);
            return &imgs.back();
        }
    }

    return nullptr;
}

void Slot::FillImage(const TexInfo& info) {
    uint8_t* tmp = m_CommonBuffer.get();
    for (uint32_t i = 0; i < info.GetTex()->GetHeight(); i++) {
        int offset1 = (int)((TexInfo::WIDTH_MAX_SIZE * (i + info.GetYOffset())) + info.GetXOffset()) * 4;
        int offset2 = (int)(info.GetTex()->GetWidth() * i) * 4;
        memcpy(tmp + offset1,
               info.GetTex()->GetBitmap() + offset2,
               info.GetTex()->GetWidth() * TexInfo::BPP_MAX_LEN);
    }
}

void Slot::FillRow(uint32_t key) {
    auto &row = m_Rows[key];

    for (auto &info : row.images) {
        FillImage(info);
    }
}

const TexInfo* Slot::PushBack(const TexInfo& info) {
    if (m_XPen + info.GetTex()->GetWidth() > TexInfo::WIDTH_MAX_SIZE) {
        m_Rows[m_YPen].maxHeight = m_MaxHeight;

        Sort(m_YPen);
#ifdef __GLIB_DEBUG__
//        FillRow(m_YPen);
#endif
        m_RowsThatNeedToReload.push(m_YPen);
        Cut(m_YPen);

        m_XPen = 0;
        m_YPen += m_MaxHeight + 1;
        m_MaxHeight = 0;
    }

    if (m_YPen + info.GetTex()->GetHeight() > TexInfo::HEIGHT_MAX_SIZE) {
#if !defined(__GLIB_DEBUG__)
        m_CommonBuffer = nullptr;
#endif
        return FindFreeSpace(info);
    }

    m_Rows[m_YPen].images.emplace_back(info.GetTex(), m_XPen, m_YPen, info.GetSlot());

    m_XPen += info.GetTex()->GetWidth() + 1;
    m_MaxHeight = std::max((int)m_MaxHeight, info.GetTex()->GetHeight());

#ifdef __GLIB_DEBUG__
//    FillImage(m_Rows[m_YPen].images.back());
#endif

    return &m_Rows[m_YPen].images.back();
}

uint32_t Slot::GetReloadRow() {
    uint32_t val = m_RowsThatNeedToReload.top();
    m_RowsThatNeedToReload.pop();
    return val;
}

uint32_t Slot::CountReloadRows() {
    return m_RowsThatNeedToReload.size();
}

const uint8_t* Slot::GetData() const {
    return m_CommonBuffer.get();
}

std::unordered_map<uint32_t, Row> &Slot::GetInfo() {
    return m_Rows;
}







TextureManager::TextureManager() {
    m_Textures = GlCore::TextureArray(TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, LAYERS);
}

void TextureManager::Bind() {
    m_Textures.Bind();
}

const TexInfo& TextureManager::PushTexture(const Texture *t) {
    if (t->GetHeight() > 3000 || t->GetWidth() > 3000) {
        ;
    }

    for (uint32_t i = FIRST_SLOT; i < LAYERS; i++) {
        m_LastCreatedEl = {t, 0, 0, i};

        auto &it = m_TexsInfo[i];

        if (const TexInfo* info = it.PushBack(m_LastCreatedEl)) {
            m_LastCreatedEl = *info;

            while (it.CountReloadRows()) {
                auto &row = it.GetInfo()[it.GetReloadRow()];

                for (const auto &image : row.images) {
                    m_Textures.LoadImage((char*)image.GetTex()->GetBitmap(),
                                         image.GetXOffset(), image.GetYOffset(),
                                         image.GetTex()->GetWidth(), image.GetTex()->GetHeight(),
                                         image.GetSlot());
                }
            }

            m_Textures.LoadImage((char*)info->GetTex()->GetBitmap(),
                                 info->GetXOffset(), info->GetYOffset(),
                                 info->GetTex()->GetWidth(), info->GetTex()->GetHeight(),
                                 info->GetSlot());
            break;
        }
#ifdef __GLIB_DEBUG__
        else {
//            PrintTextures(i);
        }
#endif
    }

    return m_LastCreatedEl;
}

const TexInfo& TextureManager::GetTexInfo(const glib::Texture *texture) {
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


const Texture &TextureManager::GetBasicTex() const {
    return m_BasicTexture;
}

#ifdef __GLIB_DEBUG__
void TextureManager::PrintTextures(int i) {
    std::string name = "output";
    name.append(std::to_string(i));
    name.append(".png");
    stbi_write_png(name.c_str(), TexInfo::WIDTH_MAX_SIZE, TexInfo::HEIGHT_MAX_SIZE, 4,
                   m_TexsInfo[i].GetData(), TexInfo::WIDTH_MAX_SIZE * 4);
}
#endif

GLIB_NAMESPACE_CLOSE