#pragma once

#include <cinttypes>
#include <vector>
#include <memory>

#include "environment.h"
#include "structs.h"

#include "GraphicsAPI/graphics_api.h"

VLADLIB_NAMESPACE_OPEN

class TexInfo {
public:
    static TexInfo SimpleTex() {
        TexInfo t {{0.f, 0.f, 1.f, 1.f}, 0};
        return t;
    }
    Rectangle atlasBounds {};
    uint32_t atlasSlot = 0;
};


using TexInfoPtr = std::shared_ptr<TexInfo>;

class TexInfoConstRef {
public:
    TexInfoConstRef() = default;
    TexInfoConstRef(const TexInfoConstRef& t) = default;

    TexInfoConstRef(TexInfoPtr* texInfo) { if (texInfo) m_TI = *texInfo; }
    bool IsValid() const { return m_TI.get(); };
    const TexInfo* operator->() const { return m_TI.get(); }
    TexInfo operator*() const { return *m_TI; }
private:
    TexInfoPtr m_TI;
};

class Slot {
public:
    Slot() = default;
    Slot(Slot&&) = default;
    ~Slot() = default;

    Slot& operator=(Slot&&) = default;

    bool PushBack(const GAPI::ImageInfo* texture);
    TexInfoPtr GetTexInfo(const GAPI::ImageInfo* texture) const;

    void SetSlotSize(uint32_t w, uint32_t h);
    uint32_t GetSlotWidth() const;
    uint32_t GetSlotHeight() const;
private:
    uint32_t m_W = 0;
    uint32_t m_H = 0;

    glm::vec<2, uint32_t> m_Pen {0, 0};
    uint32_t m_MaxRowH = 0;

    std::unordered_map<const GAPI::ImageInfo*, TexInfoPtr> m_Textures;
};

class TextureAtlas {
public:
    TextureAtlas(std::shared_ptr<GAPI::TextureArray> texArr);

    TexInfoConstRef GetTexInfo(const GAPI::ImageInfo* texture);
    const GAPI::TextureArray* GetTextureObject() const;
private:
    std::shared_ptr<GAPI::TextureArray> m_TextureObject;
    std::vector<Slot> m_Slots;

    TexInfoPtr m_SingleTexture = std::make_shared<TexInfo>();
};

VLADLIB_NAMESPACE_CLOSE
