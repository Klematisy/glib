#include "Texture.h"

GlCore::Texture::Texture() {
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    uint32_t whitePixel = 0xFFFFFFFF;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whitePixel);

    UnBind();
}

GlCore::Texture& GlCore::Texture::operator=(Texture &&other) {
    m_TextureID   = other.m_TextureID;
    m_LocalBuffer = other.m_LocalBuffer;
    m_Height      = other.m_Height;
    m_Width       = other.m_Width;
    m_BPP         = other.m_BPP;

    other.m_TextureID   = 0;
    other.m_LocalBuffer = 0;
    other.m_Height      = 0;
    other.m_Width       = 0;
    other.m_BPP         = 0;

    return *this;
}

void GlCore::Texture::PutImage(const char *filePath) {

    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(filePath, &m_Width, &m_Height, &m_BPP, 4);

    glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);

    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
}

void GlCore::Texture::Bind(uint32_t slot = 0) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void GlCore::Texture::UnBind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

GlCore::Texture::~Texture() {
    if (m_TextureID != 0)
        glDeleteTextures(1, &m_TextureID);
}

