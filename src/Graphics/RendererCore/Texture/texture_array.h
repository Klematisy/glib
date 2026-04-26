#pragma once

#include <iostream>
#include <cinttypes>

#include "Graphics/GraphicsAPI/graphics_api_impl.h"
#include "Logger/logger.h"
#include "image_info.h"

namespace RendererCore {
    class Framebuffer;

    class TextureArray : public ITexture {
    public:
        TextureArray();
        TextureArray(const TextureArray& other) = default;
        TextureArray(TextureArray&& other) noexcept;

        ~TextureArray();

        TextureArray& operator=(const TextureArray& other) = default;
        TextureArray& operator=(TextureArray&& other);

        void AddImage(const ImageInfo& info, uint32_t xOffset, uint32_t yOffset, uint32_t slot);
        void Init(uint32_t width, uint32_t height, uint32_t layersCount, const TextureParameters& tp = {});

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
        uint32_t GetLayersCount() const;

        const TextureParameters& GetTexParameters() const override;
        void SetTexParameters(const TextureParameters& tp) override;

        void Bind(uint32_t slot = 0) const override;
        void UnBind() const override;

        friend void AttachTextureArrayToFramebuffer(const Framebuffer& fb, const TextureArray& tex, GAPI::INTERNAL_FORMAT attachment, uint32_t layer);
    private:
        uint32_t m_W = 0;
        uint32_t m_H = 0;
        uint32_t m_Layers = 0;
    };
}