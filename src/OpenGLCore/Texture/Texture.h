#pragma once

#include "GL/glew.h"
#include "stb/stb_image.h"

#include "../Logger.h"

namespace GlCore {

    class Texture {
    public:
        Texture();
        ~Texture();
        Texture(const Texture& other) = default;
        Texture(Texture&& other);
        Texture &operator=(Texture &&other);

        void LoadImage(const char *filePath);
        bool IsEqual(const Texture &other) const;
        int GetWidth() const;
        int GetHeight() const;
        void Bind(uint32_t slot) const;
        void UnBind();

    private:
        int m_Width = 0,
            m_Height = 0,
            m_BPP = 0;
        unsigned char *m_LocalBuffer = nullptr;
        uint32_t m_TextureID = 0;
    };

}