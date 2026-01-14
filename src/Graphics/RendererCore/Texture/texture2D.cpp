#include <iostream>

#include "texture2D.h"

using namespace RendererCore;
using namespace GAPI;

static auto& gapi = GraphicsAPIImpl::Get();


Texture2D::Texture2D()
    : ITexture()
{
    gapi.CreateTextures(1, &m_ID);
}

Texture2D::Texture2D(const TextureParameters& tp)
    : ITexture()
{
    gapi.CreateTextures(1, &m_ID);
    Bind();

    m_TexParameters = tp;
    gapi.TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::MAG_FILTER, tp.magFilter);
    gapi.TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::MIN_FILTER, tp.minFilter);
    gapi.TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::WRAP_S, tp.wrapS);
    gapi.TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::WRAP_T, tp.wrapT);

    UnBind();
}

Texture2D::Texture2D(Texture2D&& other)
    : ITexture()
{
    m_ID = other.m_ID;
    other.m_ID = 0;
}

Texture2D::~Texture2D() {
    gapi.DeleteTextures(1, &m_ID);
}

Texture2D& Texture2D::operator=(Texture2D&& other) {
    m_ID = other.m_ID;
    other.m_ID = 0;

    return *this;
}

void Texture2D::Bind(uint32_t slot) const {
    gapi.ActiveTexture(slot);
    gapi.BindTexture(TEXTURE_TYPE::_2D, m_ID);
}

void Texture2D::UnBind() const {
    gapi.BindTexture(TEXTURE_TYPE::_2D, 0);
}

void Texture2D::Upload(const ImageInfo &info) {
    Bind();

    if (m_AllocatedW < info.GetWidth() || m_AllocatedH < info.GetHeight()) {
        gapi.TexImage2D(TEXTURE_TYPE::_2D, 0, INTERNAL_FORMAT::RGBA8, info.GetWidth(), info.GetHeight(), 0, FORMAT::RGBA, API_TYPE::UCHAR, info.GetBitmap().get());
        m_AllocatedW = info.GetWidth();
        m_AllocatedH = info.GetHeight();
    } else
        gapi.TexSubImage2D(TEXTURE_TYPE::_2D, 0, 0, 0, info.GetWidth(), info.GetHeight(), FORMAT::RGBA, API_TYPE::UCHAR, info.GetBitmap().get());

    UnBind();
}

const TextureParameters& Texture2D::GetTexParameters() const {
    return m_TexParameters;
}

void Texture2D::SetTexParameters(const TextureParameters &tp) {
    Bind();

    m_TexParameters = tp;
    gapi.TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::MAG_FILTER, tp.magFilter);
    gapi.TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::MIN_FILTER, tp.minFilter);
    gapi.TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::WRAP_S, tp.wrapS);
    gapi.TexParameteri(TEXTURE_TYPE::_2D, GAPI::TEXTURE_PROPERTY::WRAP_T, tp.wrapT);

    UnBind();
}