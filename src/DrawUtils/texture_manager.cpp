#include "texture_manager.h"
#include "GraphicsAPI/graphics_api.h"
#include "Logger/logger.h"

VLADLIB_NAMESPACE_USING;

size_t TextureParamHasher::operator()(const GAPI::TextureParameters &params) const noexcept {
    std::string p;
    p += std::to_string((int) params.wrapT);
    p += std::to_string((int) params.wrapS);
    p += std::to_string((int) params.magFilter);
    p += std::to_string((int) params.minFilter);

    return std::stoi(p);
}

void TextureManager::RegisterAtlas(const GAPI::TextureParameters& tp) {
    if (m_Atlases.find(tp) != m_Atlases.cend()) {
        LOGWARN("Texture Manager: The texture Atlas with this parameters already registered");
        return;
    }

    std_sptr<GAPI::TextureArray> textureArray = GAPI::createTexutreArray();
    textureArray->Init(m_Width, m_Height, 16, tp);
    m_Atlases[tp] = std::make_shared<TextureAtlas>(textureArray);
}

void TextureManager::RegisterTextureInstance(const GAPI::ImageInfo* info, const GAPI::ITexture* instance) {
    m_Textures[info] = instance;
}

TexInfoConstRef TextureManager::GetTextureInformation(const GAPI::ImageInfo& info) {
    if (!info.GetBitmap()) {
        return {};
    }

    if (m_Atlases.find(info.GetTexParams()) == m_Atlases.cend()) {
        RegisterAtlas(info.GetTexParams());
    }

    return m_Atlases[info.GetTexParams()]->GetTexInfo(&info);
}

const GAPI::ITexture* TextureManager::GetTextureObject(const GAPI::ImageInfo& info) {
    if (!info.GetBitmap()) {
        if (m_Textures.find(&info) == m_Textures.cend()) {
            LOGWARN("Texture Manager: The texture object didn't found");
        }
        return m_Textures[&info];
    }

    return m_Atlases[info.GetTexParams()]->GetTextureObject();
}
