#pragma once

#include <iostream>
#include <cinttypes>

#include "GL/glew.h"
#include "Logger/logger.h"

namespace GlCore {

    class TextureArray {
    public:
        TextureArray();
        TextureArray(uint32_t width, uint32_t height, uint32_t layers = 8);
        ~TextureArray();

        TextureArray(const TextureArray& other) = default;
        TextureArray(TextureArray&& other);

        TextureArray& operator=(const TextureArray& other) = default;
        TextureArray& operator=(TextureArray&& other);

        void LoadImage(char* bitmap, uint32_t slot = 0);
        void LoadImage(char* bitmap, uint32_t xOffset, uint32_t yOffset, uint32_t width , uint32_t height, uint32_t slot) const;
        void SetSize(uint32_t width, uint32_t height);
        uint32_t GetSize() const;
        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
        void Bind() const;
    private:
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_LayerCount = 0;

        uint32_t m_ID = 0;
    };

}