#include <utility>
#include <mutex>

#include "buffer_drawer.h"
#include "stb/stb_image_write.h"
#include "texture_manager.h"
#include "texture.h"

template<class T>
using vec = std::vector<T>;

template<class KEY, class VALUE>
using map = std::unordered_map<KEY, VALUE>;

GLIB_NAMESPACE_OPEN

void Slot::Sort(uint32_t key) {
    auto& unsortedRow = m_Rows[key].images;

    // TODO: need to make more optimal sort
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
    for (auto& it : unsortedRow) {
        it.SetXOffset(w);
        w += it.GetTex()->GetWidth();
        w += 1;
    }
}

void Slot::Cut(uint32_t key) {
    const auto& row = m_Rows[key];
    const auto& sortedRow = row.images;

    for (uint32_t i = 1; i < sortedRow.size(); i++) {
        const auto& PastImage = sortedRow[i - 1];
        const auto& NowImage = sortedRow[i];

        if (NowImage.GetYOffset() == row.maxHeight) continue;

        glm::vec2 extremumPoint(NowImage.GetXOffset(),
                                NowImage.GetYOffset() + NowImage.GetTex()->GetHeight());

        m_FreeRects.push_back({extremumPoint.x, extremumPoint.y,
                               (float)(m_Width - extremumPoint.x),
                               (float)(PastImage.GetTex()->GetHeight() - NowImage.GetTex()->GetHeight())
        });
    }

    auto& lastEl = sortedRow.back();

    glm::vec2 extremumPoint(lastEl.GetXOffset() + lastEl.GetTex()->GetWidth(),
                            lastEl.GetYOffset());

    if (extremumPoint.x == m_Width) return;

    m_FreeRects.push_back({extremumPoint.x, extremumPoint.y,
                           m_Width - extremumPoint.x,
                           (float)lastEl.GetTex()->GetHeight()});
}

const TexInfo* glib::Slot::FindFreeSpace(const TexInfo& tex) {
    for (uint32_t i = 0; i < m_FreeRects.size(); i++) {
        if (tex.GetTex()->GetWidth() <= m_FreeRects[i].width &&
            tex.GetTex()->GetHeight() <= m_FreeRects[i].height) {

            auto& imgs = m_Rows[(uint32_t)m_FreeRects[i].y].images;
            imgs.emplace_back(tex.GetTex(), (uint32_t)m_FreeRects[i].x, (uint32_t)m_FreeRects[i].y, tex.GetSlot());
            FillImage(imgs.back());

            Cut((uint32_t)m_FreeRects[i].y);
            m_FreeRects.erase(m_FreeRects.cbegin() + i);

            Sort((uint32_t) m_FreeRects[i].y);
            return &imgs.back();
        }
    }

    return nullptr;
}

void Slot::FillImage(const TexInfo& info) {
    if (!info.GetTex()->GetBitmap()) return;

    uint8_t* tmp = m_CommonBuffer.get();
    for (uint32_t i = 0; i < info.GetTex()->GetHeight(); i++) {
        int offset1 = (int)((m_Width * (i + info.GetYOffset())) + info.GetXOffset()) * 4;
        int offset2 = (int)(info.GetTex()->GetWidth() * i) * 4;
        memcpy(tmp + offset1,
               info.GetTex()->GetBitmap() + offset2,
               info.GetTex()->GetWidth() * 4);
    }
}

void Slot::FillRow(uint32_t key) {
    auto& row = m_Rows[key];

    for (auto& info : row.images)
        FillImage(info);
}

const TexInfo* Slot::PushBack(const TexInfo& info) {
    if (m_XPen + info.GetTex()->GetWidth() > m_Width) {
        m_Rows[m_YPen].maxHeight = m_MaxHeight;

        Sort(m_YPen);
#ifdef __GLIB_DEBUG__
        FillRow(m_YPen);
#endif
        m_RowsThatNeedToReload.push(m_YPen);
        Cut(m_YPen);

        m_XPen = 0;
        m_YPen += m_MaxHeight + 1;
        m_MaxHeight = 0;
    }

    if (m_YPen + info.GetTex()->GetHeight() > m_Height) {
#if !defined(__GLIB_DEBUG__)
        m_CommonBuffer = nullptr;
#endif
        return FindFreeSpace(info);
    }

    m_Rows[m_YPen].images.emplace_back(info.GetTex(), m_XPen, m_YPen, info.GetSlot());

    m_XPen += info.GetTex()->GetWidth() + 1;
    m_MaxHeight = std::max((int)m_MaxHeight, info.GetTex()->GetHeight());

#ifdef __GLIB_DEBUG__
    FillImage(m_Rows[m_YPen].images.back());
#endif

    return &m_Rows[m_YPen].images.back();
}

uint32_t Slot::GetReloadRow() {
    uint32_t val = m_RowsThatNeedToReload.top();
    m_RowsThatNeedToReload.pop();
    return val;
}

void Slot::SetSize(uint32_t w, uint32_t h) {
    m_Width = w;
    m_Height = h;
}

void Slot::Allocate() {
#ifdef __GLIB_DEBUG__
    m_CommonBuffer = std::unique_ptr<uint8_t>((uint8_t*)std::calloc(m_Width * m_Height * 4, 1));
#endif
}

uint32_t Slot::CountReloadRows() {
    return m_RowsThatNeedToReload.size();
}

const uint8_t* Slot::GetData() const {
    return m_CommonBuffer.get();
}

std::unordered_map<uint32_t, Row>& Slot::GetInfo() {
    return m_Rows;
}






Texture TextureManager::GetBasicTex() {
    constexpr uint32_t BASIC_TEX_WIDTH = 1;
    constexpr uint32_t BASIC_TEX_HEIGHT = 1;
    constexpr uint32_t BASIC_TEX_BPP = 4;
    constexpr uint32_t BASIC_TEX_SIZE = BASIC_TEX_WIDTH * BASIC_TEX_HEIGHT * BASIC_TEX_BPP;

    auto bitmap = std::shared_ptr<unsigned char>((unsigned char*) std::calloc(BASIC_TEX_SIZE, 1));

    for (uint32_t i = 0; i < BASIC_TEX_SIZE; i++)
        bitmap.get()[i] = 255;

    return std::move(Texture(BASIC_TEX_WIDTH, BASIC_TEX_HEIGHT, BASIC_TEX_BPP, bitmap));
}

void TextureManager::Bind() const {
    m_Textures->Bind(0);
}

static void LoadImage(RendererCore::TextureArray& texArr, const TexInfo& info) {
    if (!info.GetTex()->GetBitmap()) return;
    texArr.LoadImage((char*)info.GetTex()->GetBitmap(), info.GetSlot(),
                            info.GetXOffset(), info.GetYOffset(),
                            info.GetTex()->GetWidth(), info.GetTex()->GetHeight());
}

const TexInfo& TextureManager::PushTexture(const Texture* t) {
    int w = m_Textures->GetWidth();
    int h = m_Textures->GetHeight();
    assert(!(t->GetHeight() > w || t->GetWidth() > h) || m_Textures->GetLayersCount() == 1);

    for (uint32_t i = FIRST_SLOT; i < m_Textures->GetLayersCount(); i++) {
        m_LastCreatedEl = {t, 0, 0, i};

        auto& it = m_TexsInfo[i];

        if (const TexInfo* info = it.PushBack(m_LastCreatedEl)) {
            m_LastCreatedEl = *info;
            while (it.CountReloadRows()) {
                auto& row = it.GetInfo()[it.GetReloadRow()];
                for (const auto& image : row.images) {
                    LoadImage(*m_Textures, image);
                }
            }

            LoadImage(*m_Textures, *info);
            break;
        }
    }

    return m_LastCreatedEl;
}

void TextureManager::Clear() {
    m_TexsInfo.clear();
}

const TexInfo& TextureManager::GetTexInfo(const Texture* texture) {
    for (uint32_t i = FIRST_SLOT; i < m_Textures->GetLayersCount(); i++) {
        auto& it = m_TexsInfo[i];
        for (auto& row : it.GetInfo()) {
            for (auto& info : row.second.images) {
                if (texture == info.GetTex()) {
                    return info;
                }
            }
        }
    }

#ifdef __GLIB_DEBUG__
    bool a = 0;
    if (a) PrintTextures(1);
#endif

    return PushTexture(texture);
}

void TextureManager::SetTextureArray(std::shared_ptr<RendererCore::TextureArray>& texArr) {
    if (m_Textures)
        Logger::LogWar("TEXTURE MANAGER", "TextureArray already isn't null!");

    m_Textures = texArr;
    m_TexsInfo.clear();
    m_TexsInfo.resize(texArr->GetLayersCount() + FIRST_SLOT);

    for (auto& slot : m_TexsInfo) {
        slot.SetSize(texArr->GetWidth(), texArr->GetHeight());
        slot.Allocate();
    }
}

std::shared_ptr<RendererCore::TextureArray> TextureManager::GetTexArray() const {
    return m_Textures;
}

#ifdef __GLIB_DEBUG__
void TextureManager::PrintTextures(int i) {
    std::string name = "output";
    name.append(std::to_string(i));
    name.append(".png");
    int w = m_Textures->GetWidth();
    int h = m_Textures->GetHeight();

    stbi_write_png(name.c_str(), w, h, 4,
                   m_TexsInfo[i].GetData(), w * 4);
}
#endif

GLIB_NAMESPACE_CLOSE