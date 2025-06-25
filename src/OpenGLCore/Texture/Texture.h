#pragma once

#include "GL/glew.h"
#include "stb/stb_image.h"

namespace GlCore {

    class Texture {
    public:
        Texture();

        ~Texture();

        Texture &operator=(Texture &&other);

        void PutImage(const char *filePath);

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