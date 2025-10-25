#include "texture_array.h"

using namespace RendererCore;
using namespace GAPI;

static GraphicsAPIImpl& gapi = GraphicsAPIImpl::Get();

TextureArray::TextureArray()
{
    gapi.CreateTextures(1, &m_TextureId);
    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_TextureId);

    m_LayerCount = gapi.GetMaxArrayTexLayers();
}

TextureArray::TextureArray(uint32_t width, uint32_t height, uint32_t layers)
{
    m_Width = width;
    m_Height = height;
    m_LayerCount = (gapi.GetMaxArrayTexLayers() > layers) ? layers : m_LayerCount;

    gapi.CreateTextures(1, &m_TextureId);
    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_TextureId);
}

TextureArray::TextureArray(TextureArray &&other)
{
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_LayerCount = other.m_LayerCount;
    m_TextureId = other.m_TextureId;

    other.m_Width = 0;
    other.m_Height = 0;
    other.m_LayerCount = 0;
    other.m_TextureId = 0;
}

TextureArray& TextureArray::operator=(TextureArray &&other) {
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_LayerCount = other.m_LayerCount;
    m_TextureId = other.m_TextureId;

    other.m_Width = 0;
    other.m_Height = 0;
    other.m_LayerCount = 0;
    other.m_TextureId = 0;

    return *this;
}

TextureArray::~TextureArray() {
    gapi.DeleteTextures(1, &m_TextureId);
}

void TextureArray::Parameteri(GAPI::TEXTURE_PROPERTY texProp, GAPI::TEXTURE_PARAM texParam) {
    gapi.TexParameteri(GAPI::TEXTURE_TYPE::ARRAY, texProp, texParam);
}

void TextureArray::AllocateTexture() {
    gapi.TexImage3D(TEXTURE_TYPE::ARRAY, 0, INTERNAL_FORMAT::RGBA8,
                    (int) m_Width, (int) m_Height, (int) m_LayerCount,
                    0, INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, nullptr);
}

void TextureArray::LoadImage(char* bitmap, uint32_t slot,
                             uint32_t xOffset, uint32_t yOffset,
                             uint32_t width, uint32_t height)
{
    if (slot >= m_LayerCount) {
        Logger::LogErr("TEXTURE ARRAY", "Slot index out of range!");
        return;
    }

    width = (width == 0) ? m_Width : width;
    height = (height == 0) ? m_Height : height;

    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_TextureId);
    gapi.TexSubImage3D(TEXTURE_TYPE::ARRAY, 0,
                    (int) xOffset, (int) yOffset, (int) slot,
                    (int) width, (int) height, 1,
                    INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, bitmap);
}

void TextureArray::Bind(uint32_t slot) const {
    if (m_TextureId == 0) {
        std::cerr << "TextureArray is empty!" << std::endl;
        return;
    }
    gapi.ActiveTexture(slot);
    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_TextureId);
}

void TextureArray::UnBind() const {
    gapi.BindTexture(TEXTURE_TYPE::ARRAY, 0);
}

uint32_t TextureArray::GetLayersCount() const { return m_LayerCount; }
void TextureArray::SetLayersCount(uint32_t layers) {
    uint32_t maxLayers = gapi.GetMaxArrayTexLayers();
    if (layers > maxLayers) {
        m_LayerCount = maxLayers;
        Logger::LogWar("TEXTURE ARRAY", "You specified layers count more than your PC support. "
                                        "That's why layer count will be equal max layer count on your PC");
        return;
    }

    m_LayerCount = layers;
}

uint32_t TextureArray::GetWidth() const { return m_Width; }
uint32_t TextureArray::GetHeight() const { return m_Height; }
void TextureArray::SetWidth(uint32_t width) { m_Width = width; }
void TextureArray::SetHeight(uint32_t height) { m_Height = height; }
