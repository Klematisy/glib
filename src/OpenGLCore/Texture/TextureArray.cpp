#include "TextureArray.h"

GlCore::TextureArray::TextureArray()
    : m_Width(0), m_Height(0)
{
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);

    int layerCount;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &layerCount);
    m_LayerCount = layerCount;

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

GlCore::TextureArray::TextureArray(uint32_t width, uint32_t height, uint32_t layers)
    : m_Width(width), m_Height(height)
{
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);

    int layerCount;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &layerCount);

    m_LayerCount = (layerCount > layers) ? layers : layerCount;

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8,
                 (int) width, (int) height, (int) m_LayerCount,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

GlCore::TextureArray::TextureArray(GlCore::TextureArray &&other)
    : m_Width(other.m_Width), m_Height(other.m_Height),
      m_LayerCount(other.m_LayerCount), m_ID(other.m_ID)
{
    other.m_Width = 0;
    other.m_Height = 0;
    other.m_LayerCount = 0;
    other.m_ID = 0;
}

GlCore::TextureArray &GlCore::TextureArray::operator=(GlCore::TextureArray &&other) {
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

GlCore::TextureArray::~TextureArray() {
    if (m_ID != 0) glDeleteTextures(1, &m_ID);
}

#include "stb/stb_image_write.h"

void GlCore::TextureArray::LoadImage(char *bitmap, uint32_t slot) {
    if (slot >= m_LayerCount) {
        Logger::LogErr("Slot index out of range!");
        return;
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                    0, 0, (int) slot,
                    (int) m_Width, (int) m_Height,  1,
                    GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
}

void GlCore::TextureArray::LoadImage(uint32_t xOffset, uint32_t yOffset,
                                     uint32_t width, uint32_t height, uint32_t slot,
                                     unsigned char *image)
{
    if (slot >= m_LayerCount) {
        Logger::LogErr("Slot index out of range!");
        return;
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);

    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                    (int) xOffset, (int) yOffset, (int) slot,
                    (int) width, (int) height,  1,
                    GL_RGBA, GL_UNSIGNED_BYTE, image);
}

void GlCore::TextureArray::SetSize(uint32_t width, uint32_t height) {
    if (!(width == 0 || height == 0)) return;

    m_Width  = width;
    m_Height = height;
}

uint32_t GlCore::TextureArray::GetWidth() const {
    return m_Width;
}

uint32_t GlCore::TextureArray::GetHeight() const {
    return m_Height;
}

void GlCore::TextureArray::Bind() const {
    if (m_ID == 0) {
        std::cerr << "TextureArray is empty!" << std::endl;
        return;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);
}

uint32_t GlCore::TextureArray::GetSize() const {
    return m_LayerCount;
}
