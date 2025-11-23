#include "stb/stb_image.h"
#include "texture.h"

GLIB_NAMESPACE_OPEN

Texture::Texture(const char* filePath) {
    m_Bitmap = std::shared_ptr<unsigned char>(stbi_load(filePath, &m_Width, &m_Height, &m_BPP, 4));

    using namespace std::string_literals;

    if (m_Bitmap)
        Logger::LogInf("TEXTURE", "'"s + filePath + "' has loaded");
    else
        Logger::LogErr("TEXTURE", "'"s + filePath + "' hasn't loaded");
}

Texture::Texture(int width, int height, int bpp, const std::shared_ptr<unsigned char>& bitmap) {
    if (!bitmap) {
        Logger::LogWar("TEXTURE", "Bitmap is empty!");
    }

    m_Width  = width;
    m_Height = height;
    m_Bitmap = bitmap;

    Logger::LogInf("TEXTURE", "the bitmap has loaded");
}

int Texture::GetWidth() const {
    return m_Width;
}

int Texture::GetHeight() const {
    return m_Height;
}

uint8_t* Texture::GetBitmap() const {
    return m_Bitmap.get();
}

int Texture::GetBPP() const {
    return m_BPP;
}

uint32_t Texture::GetSize() const {
    return m_Height * m_Width * m_BPP;
}

Texture::Texture(Texture&& other) noexcept
    : m_Bitmap(std::move(other.m_Bitmap)),
      m_Height(other.m_Height), m_Width(other.m_Width),
      m_BPP(other.m_BPP)
{
    other.m_Width = 0;
    other.m_Height = 0;
    other.m_BPP = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    m_Bitmap = std::move(other.m_Bitmap);
    m_Height = other.m_Height;
    m_Width = other.m_Width;
    m_BPP = other.m_BPP;

    other.m_Width = 0;
    other.m_Height = 0;
    other.m_BPP = 0;
    return *this;
}

Texture& Texture::operator=(const Texture& other) {
    m_Bitmap = other.m_Bitmap;
    m_Height = other.m_Height;
    m_Width = other.m_Width;
    m_BPP = other.m_BPP;

    return *this;
}

void Texture::SetNewTexInfo(int width, int height, int bpp) {
    m_Width = width;
    m_Height = height;
    m_BPP = bpp;
}

GLIB_NAMESPACE_CLOSE