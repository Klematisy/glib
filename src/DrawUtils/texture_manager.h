#pragma once

#include <cinttypes>
#include <vector>
#include <stack>
#include <memory>

#include "environment.h"
#include "structs.h"

#include "Graphics/GraphicsAPI/graphics_api_impl.h"
#include "Graphics/RendererCore/Texture/image_info.h"


GLIB_NAMESPACE_OPEN

using Tex2d = RendererCore::ImageInfo;

struct TexArrElInfo {
    static constexpr uint32_t WIDTH_MAX_SIZE  = 3000;
    static constexpr uint32_t HEIGHT_MAX_SIZE = 3000;
};

class TexInfo {
public:
    TexInfo() = default;

    const Rectangle& GetRectangle() const { return m_ImageRect; }
    uint32_t GetSlot() const { return m_Slot; }

    void SetImageRectangle(const Rectangle& infoRect) { m_ImageRect = infoRect; }
    void SetSlot(uint32_t slot) { m_Slot = slot; }
private:
    Rectangle m_ImageRect;
    uint32_t m_Slot = 0;
};


using TexInfoPtr = std::shared_ptr<TexInfo>;

class TexInfoConstRef {
public:
    TexInfoConstRef(TexInfoPtr& texInfo) { m_TI = texInfo; }
    const TexInfo* operator->() const { return m_TI.get(); }
private:
    TexInfoPtr m_TI;
};

class Slot {
public:
    Slot() = default;
    Slot(Slot&&) = default;
    ~Slot() = default;

    Slot& operator=(Slot&&) = default;

    bool PushBack(const Tex2d* texture);
    TexInfoPtr GetTexInfo(const Tex2d* texture) const;

    void SetSlotSize(uint32_t w, uint32_t h);
    uint32_t GetSlotWidth() const;
    uint32_t GetSlotHeight() const;
private:
    uint32_t m_W = 0;
    uint32_t m_H = 0;

    glm::vec<2, uint32_t> m_Pen {0, 0};
    uint32_t m_MaxRowH = 0;

    std::unordered_map<const Tex2d*, TexInfoPtr> m_Textures;
};

class TextureManager {
public:
    TextureManager(std::shared_ptr<RendererCore::ITexture>& texArr);

    TexInfoConstRef GetTexInfo(const Tex2d* texture);
    const RendererCore::ITexture* GetTextureObject() const;

    void Bind() const;
private:
    std::shared_ptr<RendererCore::ITexture> m_TextureObject;
    std::vector<Slot> m_Slots;

    TexInfoPtr m_SingleTexture = std::make_shared<TexInfo>();
};

GLIB_NAMESPACE_CLOSE