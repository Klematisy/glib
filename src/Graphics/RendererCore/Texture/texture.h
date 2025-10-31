#pragma once

#include "../../GraphicsAPI/graphics_api_impl.h"

namespace RendererCore {
    class ITexture {
    public:
        virtual void Parameteri(GAPI::TEXTURE_PROPERTY texProp, GAPI::TEXTURE_PARAM texParam) = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void SetWidth(uint32_t) = 0;
        virtual void SetHeight(uint32_t) = 0;

        virtual void Bind(uint32_t slot) const = 0;
        virtual void UnBind() const = 0;

    protected:
        virtual void AllocateTexture() = 0;

        uint32_t m_TextureId = 0;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_BPP = 0;
    };
}