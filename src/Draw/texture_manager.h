#pragma once

#include <cinttypes>
#include <vector>
#include <stack>
#include <memory>

#include "environment.h"
#include "structs.h"
#include "texture.h"
#include "Graphics/GraphicsAPI/graphics_api_impl.h"


GLIB_NAMESPACE_OPEN

class TexInfo {
public:
    static constexpr uint32_t WIDTH_MAX_SIZE  = 3000;
    static constexpr uint32_t HEIGHT_MAX_SIZE = 3000;
    static constexpr uint32_t BPP_MAX_LEN = 4;

    static constexpr uint32_t BUFFER_MAX_SIZE =
            WIDTH_MAX_SIZE * HEIGHT_MAX_SIZE * BPP_MAX_LEN;

    TexInfo() = default;
    TexInfo(const Texture* tex, uint32_t xOffset, uint32_t yOffset, uint32_t slot = 0)
            : m_Tex(tex), m_XOffset(xOffset), m_YOffset(yOffset), m_Slot(slot)
    {}

    const Texture* GetTex() const { return m_Tex;     }
    uint32_t GetXOffset()   const { return m_XOffset; }
    uint32_t GetYOffset()   const { return m_YOffset; }
    uint32_t GetSlot()      const { return m_Slot;    }

    void SetTex(const Texture* tex) { m_Tex = tex;   }
    void SetXOffset(uint32_t x)     { m_XOffset = x; }
    void SetYOffset(uint32_t y)     { m_YOffset = y; }
    void SetSlot(uint32_t slot)     { m_Slot = slot; }
private:
    const Texture* m_Tex = nullptr;
    uint32_t m_XOffset   = 0;
    uint32_t m_YOffset   = 0;
    uint32_t m_Slot      = 0;
};

struct Row {
    std::vector<TexInfo> images;
    uint32_t maxHeight = 0;
    uint32_t width = 0;
};

class Slot {
public:
    Slot();
    Slot(Slot&&) = default;

    ~Slot() = default;

    Slot& operator=(Slot&&) = default;

    std::unordered_map<uint32_t, Row>& GetInfo();
    const uint8_t* GetData() const;
    const glib::TexInfo* PushBack(const TexInfo& info);

    uint32_t GetReloadRow();
    uint32_t CountReloadRows();
private:
    void Sort(uint32_t key);
    void Cut(uint32_t key);

    void FillRow(uint32_t key);
    void FillImage(const TexInfo& info);

    const glib::TexInfo* FindFreeSpace(const TexInfo& tex);
private:
    std::unordered_map<uint32_t, Row> m_Rows;

    std::unique_ptr<uint8_t> m_CommonBuffer;
    std::vector<Rectangle> m_FreeRects;

    uint32_t m_MaxHeight = 0;
    uint32_t m_XPen = 0;
    uint32_t m_YPen = 0;

    std::stack<uint32_t> m_RowsThatNeedToReload;
};

class TextureManager {
public:
    TextureManager() = default;

    void SetTextureArray(std::shared_ptr<RendererCore::TextureArray>& texArr);
    const RendererCore::TextureArray& GetTexArray() const;

    void Bind() const;
    const TexInfo& GetTexInfo(const Texture* texture);

    static Texture GetBasicTex();
    static constexpr uint32_t FIRST_SLOT = 1;
#ifdef __GLIB_DEBUG__
    void PrintTextures(int i);
#endif
private:
    const TexInfo& PushTexture(const Texture* texture);

    std::shared_ptr<RendererCore::TextureArray> m_Textures;
    std::vector<Slot> m_TexsInfo;

    TexInfo m_LastCreatedEl {0, 0, 0, 0};
};

GLIB_NAMESPACE_CLOSE