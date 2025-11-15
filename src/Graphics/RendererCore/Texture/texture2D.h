#pragma once

#include "stb/stb_image.h"

#include "Logger/logger.h"
#include "Graphics/GraphicsAPI/graphics_api_impl.h"

#include "texture.h"

namespace RendererCore {
    class FrameBuffer;
    class TextureArray;

    class Texture2D : public ITexture {
    public:
        Texture2D();
        ~Texture2D();
        Texture2D(const Texture2D& other) = default;
        Texture2D(Texture2D&& other);
        Texture2D &operator=(Texture2D&& other);

        void LoadImage(uint32_t width, uint32_t height, std::shared_ptr<unsigned char>& image);
        void Parameteri(GAPI::TEXTURE_PROPERTY texProp, GAPI::TEXTURE_PARAM texParam) override;

        uint32_t GetWidth() const override;
        uint32_t GetHeight() const override;

        void SetWidth(uint32_t width) override;
        void SetHeight(uint32_t height) override;

        void Bind(uint32_t slot) const override;
        void UnBind() const override;

        friend void AttachTextureToFramebuffer(const FrameBuffer& fb,const Texture2D& tex, GAPI::ATTACHMENT attachment);
        friend void AttachTextureArrayToFramebuffer(const FrameBuffer& fb, const TextureArray& tex, GAPI::ATTACHMENT attachment, uint32_t layer);

    private:
        void AllocateTexture() override;

        std::shared_ptr<unsigned char> m_LocalBuffer = nullptr;
    };

}