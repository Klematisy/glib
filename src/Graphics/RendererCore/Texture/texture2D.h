#pragma once

#include "stb/stb_image.h"

#include "Logger/logger.h"
#include "Graphics/GraphicsAPI/graphics_api_impl.h"

#include "image_info.h"

namespace RendererCore {
    class Framebuffer;
    class TextureArray;

    class Texture2D : public ITexture {
    public:
        Texture2D();
        Texture2D(const TextureParameters& tp);
        Texture2D(const Texture2D& other) = default;
        Texture2D(Texture2D&& other);
        ~Texture2D();

        Texture2D &operator=(Texture2D&& other);

        void Upload(const ImageInfo& info);

        const TextureParameters& GetTexParameters() const override;
        void SetTexParameters(const TextureParameters& tp) override;

        void Bind(uint32_t slot = 0) const override;
        void UnBind() const override;

        friend void AttachTextureToFramebuffer(const Framebuffer& fb,const Texture2D& tex, GAPI::INTERNAL_FORMAT attachment);
        friend void AttachTextureArrayToFramebuffer(const Framebuffer& fb, const TextureArray& tex, GAPI::INTERNAL_FORMAT attachment, uint32_t layer);

    private:
        uint32_t m_AllocatedW = 0;
        uint32_t m_AllocatedH = 0;
    };

}