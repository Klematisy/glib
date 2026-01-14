#include "texture_array.h"

using namespace RendererCore;
using namespace GAPI;

static auto& gapi = GraphicsAPIImpl::Get();

TextureArray::TextureArray(uint32_t width, uint32_t height, uint32_t layers, const TextureParameters& tp)
    : ITexture()
{
    m_W = width;
    m_H = height;
    m_Layers = layers;
    m_TexParameters = tp;

    uint32_t maxLayers = gapi.GetMaxArrayTexLayers();
    if (layers > maxLayers) {
        m_Layers = maxLayers;
        Logger::LogWar("TEXTURE ARRAY", "You specified layers count more than your PC support. "
                                        "That's why layer count will be equal max layer count on your PC");
        return;
    }

    gapi.CreateTextures(1, &m_ID);
    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_ID);

    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, GAPI::TEXTURE_PROPERTY::MAG_FILTER, tp.magFilter);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, GAPI::TEXTURE_PROPERTY::MIN_FILTER, tp.minFilter);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, GAPI::TEXTURE_PROPERTY::WRAP_S, tp.wrapS);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, GAPI::TEXTURE_PROPERTY::WRAP_T, tp.wrapT);

    gapi.TexImage3D(TEXTURE_TYPE::ARRAY, 0, INTERNAL_FORMAT::RGBA8,
                    (int) m_W, (int) m_H, (int) m_Layers,
                    0, FORMAT::RGBA, API_TYPE::UCHAR, nullptr);
}

TextureArray::TextureArray(TextureArray&& other)
    : ITexture()
{
    m_W = other.m_W;
    m_H = other.m_H;
    m_Layers = other.m_Layers;
    m_ID = other.m_ID;

    other.m_W = 0;
    other.m_H = 0;
    other.m_Layers = 0;
    other.m_ID = 0;
}

TextureArray& TextureArray::operator=(TextureArray&& other) {
    m_W = other.m_W;
    m_H = other.m_H;
    m_Layers = other.m_Layers;
    m_ID = other.m_ID;

    other.m_W = 0;
    other.m_H = 0;
    other.m_Layers = 0;
    other.m_ID = 0;

    return *this;
}

TextureArray::~TextureArray() {
    gapi.DeleteTextures(1, &m_ID);
}

void TextureArray::AddImage(const ImageInfo& info, uint32_t xOffset, uint32_t yOffset, uint32_t slot)
{
    if (slot >= m_Layers) {
        Logger::LogErr("TEXTURE ARRAY", "Slot index out of range!");
        return;
    }

    if (m_W < info.GetWidth() || m_H < info.GetHeight()) {
        Logger::LogErr("TEXTURE ARRAY", "Texture out of texture array range!");
        return;
    }

    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_ID);
    gapi.TexSubImage3D(TEXTURE_TYPE::ARRAY, 0,
                    (int) xOffset, (int) yOffset, (int) slot,
                    (int) info.GetWidth(), (int) info.GetHeight(), 1,
                    FORMAT::RGBA, API_TYPE::UCHAR, info.GetBitmap().get());
}

void TextureArray::Bind(uint32_t slot) const {
    if (m_ID == 0) {
        std::cerr << "TextureArray is empty!" << std::endl;
        return;
    }
    gapi.ActiveTexture(slot);
    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_ID);
}

void TextureArray::UnBind() const {
    gapi.BindTexture(TEXTURE_TYPE::ARRAY, 0);
}

uint32_t TextureArray::GetWidth() const { return m_W; }
uint32_t TextureArray::GetHeight() const { return m_H; }
uint32_t TextureArray::GetLayersCount() const { return m_Layers; }
const TextureParameters& TextureArray::GetTexParameters() const { return m_TexParameters; }

void TextureArray::SetTexParameters(const TextureParameters &tp) {
    Bind();

    m_TexParameters = tp;

    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, GAPI::TEXTURE_PROPERTY::MAG_FILTER, tp.magFilter);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, GAPI::TEXTURE_PROPERTY::MIN_FILTER, tp.minFilter);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, GAPI::TEXTURE_PROPERTY::WRAP_S, tp.wrapS);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, GAPI::TEXTURE_PROPERTY::WRAP_T, tp.wrapT);

    UnBind();
}

