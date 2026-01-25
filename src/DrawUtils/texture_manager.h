#pragma once

#include <iostream>
#include <unordered_map>
#include <cinttypes>

#include "Graphics/RendererCore/Texture/image_info.h"
#include "texture_atlas.h"
#include "environment.h"

GLIB_NAMESPACE_OPEN

template<class key, class value, class hasher>
using std_umap = std::unordered_map<key, value, hasher>;

template<class T>
using std_sptr = std::shared_ptr<T>;

namespace rc = RendererCore;

class TextureParamHasher {
public:
    size_t operator()(const rc::TextureParameters& params) const noexcept;
};

class TextureManager {
public:
    TextureManager() = default;
    void RegisterAtlas(const rc::TextureParameters& tp);
    void Bind(const rc::TextureParameters& tp);
    TexInfoConstRef GetTextureInformation(const rc::ImageInfo& info);

    const TextureAtlas& GetAtlas(const rc::TextureParameters& tp);
private:
    std_umap<rc::TextureParameters, std_sptr<TextureAtlas>, TextureParamHasher> m_Atlases;
};

GLIB_NAMESPACE_CLOSE