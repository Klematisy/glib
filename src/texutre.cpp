#include "glib.h"

#include "stb/stb_image.h"
#include "graphicsUtils.h"

GLIB_NAMESPACE_OPEN

Texture::Texture(const char *filePath) {
//    stbi_set_flip_vertically_on_load(1);
    m_Bitmap = stbi_load(filePath, &m_Width, &m_Height, &m_BPP, 4);

    using namespace std::string_literals;

    if (m_Bitmap) {
        Logger::LogInf("TEXTURE: '"s + filePath + "' has loaded");
    } else {
        Logger::LogErr("TEXTURE: '"s + filePath + "' hasn't loaded");
    }
}

Texture::Texture(int width, int height, uint8_t* bitmap) {
    if (!bitmap) {
        Logger::LogErr("Bitmap is null! Creating...");
        m_BPP = 4;
        uint32_t size = width * height * m_BPP;

        bitmap = (uint8_t*) std::calloc(size, 1);
        for (uint32_t i = 0; i < size; i++) {
            bitmap[i] = 255;
        }
    }

    m_Width  = width;
    m_Height = height;
    m_Bitmap = bitmap;

    Logger::LogInf("TEXTURE: a bitmap has loaded");
}

Texture::~Texture() {
    stbi_image_free(m_Bitmap);
}

int Texture::GetWidth() const {
    return m_Width;
}

int Texture::GetHeight() const {
    return m_Height;
}

uint8_t* Texture::GetBitmap() const {
    return m_Bitmap;
}

int Texture::GetBPP() const {
    return m_BPP;
}

uint32_t Texture::GetSize() const {
    return m_Height * m_Width * m_BPP;
}

Texture::Texture(Texture &&other) noexcept
    : m_Bitmap(other.m_Bitmap),
      m_Height(other.m_Height), m_Width(other.m_Width),
      m_BPP(other.m_BPP)
{
    other.m_Bitmap = nullptr;
    other.m_Width = 0;
    other.m_Height = 0;
    other.m_BPP = 0;
}

Texture &Texture::operator=(Texture &&other) noexcept {
    return *this;
}

GLIB_NAMESPACE_CLOSE