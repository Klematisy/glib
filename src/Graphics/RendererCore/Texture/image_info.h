#pragma once

#include <utility>

#include "Graphics/GraphicsAPI/graphics_api_impl.h"
#include "Logger/logger.h"
#include "stb/stb_image.h"

namespace RendererCore {
    struct TextureParameters {
        bool operator==(const TextureParameters& other) const {
            return wrapT == other.wrapT &&
                   wrapS == other.wrapS &&
                   magFilter == other.magFilter &&
                   minFilter == other.minFilter;
        }

        GAPI::TEXTURE_PARAM magFilter = GAPI::TEXTURE_PARAM::NEAREST;
        GAPI::TEXTURE_PARAM minFilter = GAPI::TEXTURE_PARAM::NEAREST;
        GAPI::TEXTURE_PARAM wrapS = GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE;
        GAPI::TEXTURE_PARAM wrapT = GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE;
    };

    class ImageInfo {
    public:
        ImageInfo() = default;
        ImageInfo(uint32_t w, uint32_t h, uint32_t bpp, std::shared_ptr<uint8_t> bitmap)
            : m_W(w), m_H(h), m_BPP(bpp), m_Bitmap(std::move(bitmap))
        {}

        explicit ImageInfo(const char* fileName) {
            m_Bitmap = std::shared_ptr<uint8_t>(
                stbi_load(fileName, (int*)&m_W, (int*)&m_H, (int*)&m_BPP, 4),
                [](uint8_t* p) {
                    stbi_image_free(p);
                }
            );
            if (!m_Bitmap) {
                using namespace std::string_literals;
                Logger::LogErr("STBi", "Image '"s + fileName + "' hasn't loaded!");
            }
        }

        uint32_t GetWidth() const { return m_W; }
        uint32_t GetHeight() const { return m_H; }
        uint32_t GetBPP() const { return m_BPP; }
        std::shared_ptr<uint8_t> GetBitmap() const { return m_Bitmap; }
        const TextureParameters& GetTexParams() const { return m_TP; }

        void SetTexParam(const TextureParameters& texParam) { m_TP = texParam; }
    private:
        uint32_t m_W = 0;
        uint32_t m_H = 0;
        uint32_t m_BPP = 0;
        std::shared_ptr<uint8_t> m_Bitmap;
        TextureParameters m_TP;
    };


    class ITexture {
    public:
        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void UnBind() const = 0;

        virtual const TextureParameters& GetTexParameters() const = 0;
        virtual void SetTexParameters(const TextureParameters& tp) = 0;
    protected:
        uint32_t m_ID = 0;
        TextureParameters m_TexParameters;
    };
}