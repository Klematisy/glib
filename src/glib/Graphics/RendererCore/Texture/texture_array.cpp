#include "texture_array.h"

using namespace RendererCore;
using namespace GAPI;

static GraphicsAPIImpl& gapi = GraphicsAPIImpl::Get();

TextureArray::TextureArray()
    : m_Width(0), m_Height(0)
{
    gapi.CreateTextures(1, &m_ID);
    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_ID);

    m_LayerCount = gapi.GetMaxArrayTexLayers();

    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, TEXTURE_PROPERTY::MIN_FILTER, TEXTURE_PARAM::LINEAR);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, TEXTURE_PROPERTY::MAG_FILTER, TEXTURE_PARAM::LINEAR);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, TEXTURE_PROPERTY::WRAP_S, TEXTURE_PARAM::CLAMP_TO_EDGE);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, TEXTURE_PROPERTY::WRAP_T, TEXTURE_PARAM::CLAMP_TO_EDGE);
}

TextureArray::TextureArray(uint32_t width, uint32_t height, uint32_t layers)
    : m_Width(width), m_Height(height)
{
    gapi.CreateTextures(1, &m_ID);
    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_ID);

    m_LayerCount = gapi.GetMaxArrayTexLayers();
    m_LayerCount = (m_LayerCount > layers) ? layers : m_LayerCount;

    gapi.TexImage3D(TEXTURE_TYPE::ARRAY, 0, INTERNAL_FORMAT::RGBA8,
                 (int) width, (int) height, (int) m_LayerCount,
                 0, INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, nullptr);

    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, TEXTURE_PROPERTY::MIN_FILTER, TEXTURE_PARAM::LINEAR);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, TEXTURE_PROPERTY::MAG_FILTER, TEXTURE_PARAM::LINEAR);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, TEXTURE_PROPERTY::WRAP_S, TEXTURE_PARAM::CLAMP_TO_EDGE);
    gapi.TexParameteri(TEXTURE_TYPE::ARRAY, TEXTURE_PROPERTY::WRAP_T, TEXTURE_PARAM::CLAMP_TO_EDGE);
}

TextureArray::TextureArray(TextureArray &&other)
    : m_Width(other.m_Width), m_Height(other.m_Height),
      m_LayerCount(other.m_LayerCount), m_ID(other.m_ID)
{
    other.m_Width = 0;
    other.m_Height = 0;
    other.m_LayerCount = 0;
    other.m_ID = 0;
}

TextureArray &TextureArray::operator=(TextureArray &&other) {
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_LayerCount = other.m_LayerCount;
    m_ID = other.m_ID;

    other.m_Width = 0;
    other.m_Height = 0;
    other.m_LayerCount = 0;
    other.m_ID = 0;

    return *this;
}

TextureArray::~TextureArray() {
    gapi.DeleteTextures(1, &m_ID);
}

#include "stb/stb_image_write.h"

void TextureArray::LoadImage(char *bitmap, uint32_t slot) {
    if (slot >= m_LayerCount) {
        Logger::LogErr("TEXTURE_ARRAY", "Slot index out of range!");
        return;
    }

    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_ID);

    gapi.TexSubImage3D(TEXTURE_TYPE::ARRAY, 0,
                    0, 0, (int) slot,
                    (int) m_Width, (int) m_Height,  1,
                    INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, bitmap);
}

void TextureArray::LoadImage(char* bitmap,
                             uint32_t xOffset, uint32_t yOffset,
                             uint32_t width , uint32_t height,
                             uint32_t slot) const
{
    if (slot >= m_LayerCount) {
        Logger::LogErr("TEXTURE ARRAY", "Slot index out of range!");
        return;
    }

    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_ID);

    gapi.TexSubImage3D(TEXTURE_TYPE::ARRAY, 0,
                    (int) xOffset, (int) yOffset, (int) slot,
                    (int) width, (int) height,  1,
                    INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, bitmap);
}

void TextureArray::SetSize(uint32_t width, uint32_t height) {
    if (!(width == 0 || height == 0)) return;

    if (m_Width < width || m_Height < height) {
        gapi.TexImage3D(TEXTURE_TYPE::ARRAY, 0, INTERNAL_FORMAT::RGBA8,
                     (int) width, (int) height, (int) m_LayerCount,
                     0, INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, nullptr);
    }

    m_Width  = width;
    m_Height = height;
}

uint32_t TextureArray::GetWidth() const {
    return m_Width;
}

uint32_t TextureArray::GetHeight() const {
    return m_Height;
}

void TextureArray::Bind() const {
    if (m_ID == 0) {
        std::cerr << "TextureArray is empty!" << std::endl;
        return;
    }
    gapi.ActiveTexture(0);
    gapi.BindTexture(TEXTURE_TYPE::ARRAY, m_ID);
}

uint32_t TextureArray::GetSize() const {
    return m_LayerCount;
}
