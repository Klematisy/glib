#pragma once

#include "../environment.h"

GLIB_NAMESPACE_OPEN

class Texture {
public:
    Texture() = default;
    Texture(Texture &&other) noexcept;
    explicit Texture(const char *filePath);
    Texture(int width, int height, uint8_t* bitmap);
    ~Texture();

    Texture& operator=(Texture &&other) noexcept;

    int GetWidth()  const;
    int GetHeight() const;
    int GetBPP()    const;

    uint32_t GetSize()   const;
    uint8_t* GetBitmap() const;
private:
    uint8_t* m_Bitmap = nullptr;
    int      m_BPP    = 0;
    int      m_Width  = 0;
    int      m_Height = 0;
};

GLIB_NAMESPACE_CLOSE