#include "texture2D.h"
#include "../renderer.h"

using namespace RendererCore;
using namespace GAPI;

static const auto gapi = rendererAPI;

Texture2D::Texture2D()
    : ITexture()
{
    gapi->CreateTextures(1, &m_ID);
}

Texture2D::Texture2D(Texture2D&& other)
    : ITexture()
{
    m_ID = other.m_ID;
    other.m_ID = 0;
}

Texture2D::~Texture2D() {
    gapi->DeleteTextures(1, &m_ID);
}

Texture2D& Texture2D::operator=(Texture2D&& other) {
    m_ID = other.m_ID;
    other.m_ID = 0;

    return *this;
}

void Texture2D::Bind(uint32_t slot) const {
    gapi->ActiveTexture(slot);
    gapi->BindTexture(TEXTURE_TYPE::_2D, m_ID);
}

void Texture2D::UnBind() const {
    gapi->BindTexture(TEXTURE_TYPE::_2D, 0);
}

void Texture2D::Upload(const ImageInfo &info) {
    Bind();

    auto& tp = m_TexParameters;
    if (tp != info.GetTexParams()) {
        tp = info.GetTexParams();

        gapi->TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::MAG_FILTER, tp.magFilter);
        gapi->TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::MIN_FILTER, tp.minFilter);
        gapi->TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::WRAP_S, tp.wrapS);
        gapi->TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::WRAP_T, tp.wrapT);
    }

    if (m_AllocatedW != info.GetWidth() || m_AllocatedH != info.GetHeight()) {
        gapi->TexImage2D(TEXTURE_TYPE::_2D, 0, INTERNAL_FORMAT::RGBA8, info.GetWidth(), info.GetHeight(), 0, FORMAT::RGBA, API_TYPE::UCHAR, info.GetBitmap().get());
        m_AllocatedW = info.GetWidth();
        m_AllocatedH = info.GetHeight();
    } else
        gapi->TexSubImage2D(TEXTURE_TYPE::_2D, 0, 0, 0, info.GetWidth(), info.GetHeight(), FORMAT::RGBA, API_TYPE::UCHAR, info.GetBitmap().get());

    m_W = info.GetWidth();
    m_H = info.GetHeight();

    UnBind();
}

const TextureParameters& Texture2D::GetTexParameters() const { return m_TexParameters; }
uint32_t Texture2D::GetWidth() const { return m_W; }
uint32_t Texture2D::GetHeight() const { return m_H; }
