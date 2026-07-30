#pragma once

#include <unordered_map>

#include "GraphicsAPI/graphics_api.h"
#include "texture_atlas.h"
#include "environment.h"

VLADLIB_NAMESPACE_OPEN

class TextureParamHasher {
public:
    size_t operator()(const GAPI::TextureParameters& params) const noexcept;
};

class TextureManager {
public:
    TextureManager(uint32_t w, uint32_t h) : m_Width(w), m_Height(h) {}
    void RegisterAtlas(const GAPI::TextureParameters& tp);
    TexInfoConstRef GetTextureInformation(const GAPI::ImageInfo& info);
    void RegisterTextureInstance(const GAPI::ImageInfo* info, const GAPI::ITexture* instance);
    const GAPI::ITexture* GetTextureObject(const GAPI::ImageInfo& info);
private:
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    std::unordered_map<GAPI::TextureParameters, std::shared_ptr<TextureAtlas>, TextureParamHasher> m_Atlases;
    std::unordered_map<const GAPI::ImageInfo*, const GAPI::ITexture*> m_Textures;
};

VLADLIB_NAMESPACE_CLOSE
