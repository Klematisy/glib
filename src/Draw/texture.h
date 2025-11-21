#pragma once

#include "environment.h"

GLIB_NAMESPACE_OPEN

class Texture {
public:
    Texture() = default;
    Texture(Texture&& other) noexcept;
    explicit Texture(const char* filePath);
    Texture(int width, int height, int bpp, const std::shared_ptr<unsigned char>& bitmap = nullptr);

    Texture& operator=(Texture&& other) noexcept;
    Texture& operator=(const Texture& other);

    int GetWidth()  const;
    int GetHeight() const;
    int GetBPP()    const;

    uint32_t GetSize()   const;
    uint8_t* GetBitmap() const;
private:
    std::shared_ptr<unsigned char> m_Bitmap;
    int      m_BPP    = 0;
    int      m_Width  = 0;
    int      m_Height = 0;
};

GLIB_NAMESPACE_CLOSE