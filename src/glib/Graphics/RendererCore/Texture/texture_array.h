#pragma once

#include <iostream>
#include <cinttypes>

#include "Graphics/GraphicsAPI/graphics_api_impl.h"
#include "Logger/logger.h"
#include "texture.h"

namespace RendererCore {
    class FrameBuffer;

    class TextureArray : public ITexture {
    public:
        TextureArray();
        ~TextureArray();
        TextureArray(uint32_t width, uint32_t height, uint32_t layers);

        TextureArray(const TextureArray& other) = default;
        TextureArray(TextureArray&& other);

        TextureArray& operator=(const TextureArray& other) = default;
        TextureArray& operator=(TextureArray&& other);

        void Parameteri(GAPI::TEXTURE_PROPERTY texProp, GAPI::TEXTURE_PARAM texParam) override;

        uint32_t GetWidth() const override;
        uint32_t GetHeight() const override;
        void SetWidth(uint32_t width) override;
        void SetHeight(uint32_t height) override;

        void LoadImage(char* bitmap, uint32_t slot, uint32_t xOffset = 0, uint32_t yOffset = 0, uint32_t width = 0, uint32_t height = 0);

        void Bind(uint32_t slot) const override;
        void Bind() const;
        void UnBind() const override;

        void AllocateTexture() override;

        uint32_t GetLayersCount() const;
        void SetLayersCount(uint32_t layers);

        friend void AttachTextureArrayToFramebuffer(const FrameBuffer& fb, const TextureArray& tex, GLenum attachment, uint32_t layer);
    private:
        uint32_t m_LayerCount = 0;
    };

}