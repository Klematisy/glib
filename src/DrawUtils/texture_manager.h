#pragma once

#include <unordered_map>
#include <cinttypes>

#include "Graphics/RendererCore/Texture/image_info.h"
#include "texture_atlas.h"
#include "environment.h"

VLADLIB_NAMESPACE_OPEN

template<class key, class value, class hasher>
using std_umap = std::unordered_map<key, value, hasher>;

template<class T>
using std_sptr = std::shared_ptr<T>;

class TextureParamHasher {
public:
    size_t operator()(const RendererCore::TextureParameters& params) const noexcept;
};

class TextureManager {
public:
    TextureManager(uint32_t w, uint32_t h) : m_Width(w), m_Height(h) {}
    void RegisterAtlas(const RendererCore::TextureParameters& tp);
    TexInfoConstRef GetTextureInformation(const RendererCore::ImageInfo& info);
    void RegisterTextureInstance(const RendererCore::ImageInfo* info, const RendererCore::ITexture* instance);
    const RendererCore::ITexture* GetTextureObject(const RendererCore::ImageInfo& info);
private:
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    std_umap<RendererCore::TextureParameters, std_sptr<TextureAtlas>, TextureParamHasher> m_Atlases;
    std::unordered_map<const RendererCore::ImageInfo*, const RendererCore::ITexture*> m_Textures;
};

VLADLIB_NAMESPACE_CLOSE