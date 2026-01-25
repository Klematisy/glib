#include "texture_manager.h"

GLIB_NAMESPACE_USING;

size_t TextureParamHasher::operator()(const rc::TextureParameters &params) const noexcept {
    std::string p;
    p += std::to_string((int) params.wrapT);
    p += std::to_string((int) params.wrapS);
    p += std::to_string((int) params.magFilter);
    p += std::to_string((int) params.minFilter);

    return std::stoi(p);
}



void TextureManager::RegisterAtlas(const rc::TextureParameters& tp) {
    if (m_Atlases.find(tp) != m_Atlases.cend()) {
        Logger::LogWar("Texture Manager", "Texture Atlas with this parameters already registered");
        return;
    }

    std_sptr<rc::TextureArray> textureArray = std::make_shared<rc::TextureArray>(3000, 3000, 16, tp);
    m_Atlases[tp] = std::make_shared<TextureAtlas>(textureArray);
}

TexInfoConstRef TextureManager::GetTextureInformation(const rc::ImageInfo &info) {
    if (m_Atlases.find(info.GetTexParams()) == m_Atlases.cend()) {
        Logger::LogErr("Texture Manager", "Texture Atlas with this parameters hasn't registered!");
        assert(0);
    }

    return m_Atlases[info.GetTexParams()]->GetTexInfo(&info);
}

void TextureManager::Bind(const rc::TextureParameters &tp) {
    m_Atlases[tp]->GetTextureObject()->Bind();
}

const TextureAtlas& TextureManager::GetAtlas(const rc::TextureParameters& tp) {
    return *m_Atlases[tp];
}
