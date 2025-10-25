#include <iostream>

#include "texture2D.h"

using namespace RendererCore;
using namespace GAPI;

static GraphicsAPIImpl& gapi = GraphicsAPIImpl::Get();

Texture2D::Texture2D() {
    gapi.CreateTextures(1, &m_TextureId);
    gapi.BindTexture(TEXTURE_TYPE::_2D, m_TextureId);

//    Parameteri(TEXTURE_PROPERTY::MIN_FILTER, TEXTURE_PARAM::LINEAR);
//    Parameteri(TEXTURE_PROPERTY::MAG_FILTER, TEXTURE_PARAM::LINEAR);
//    Parameteri(TEXTURE_PROPERTY::WRAP_S, TEXTURE_PARAM::CLAMP_TO_EDGE);
//    Parameteri(TEXTURE_PROPERTY::WRAP_T, TEXTURE_PARAM::CLAMP_TO_EDGE);

    uint32_t whitePixel = 0xFFFFFFFF;
    gapi.TexImage2D(TEXTURE_TYPE::_2D, 0, INTERNAL_FORMAT::RGB8, 1, 1, 0, INTERNAL_FORMAT::RGBA8, API_TYPE::UCHAR, &whitePixel);

    UnBind();
}

Texture2D::Texture2D(Texture2D &&other) {
    m_TextureId   = other.m_TextureId;
    m_LocalBuffer = other.m_LocalBuffer;
    m_Height      = other.m_Height;
    m_Width       = other.m_Width;
    m_BPP         = other.m_BPP;

    other.m_TextureId   = 0;
    other.m_LocalBuffer = 0;
    other.m_Height      = 0;
    other.m_Width       = 0;
    other.m_BPP         = 0;
}

Texture2D& Texture2D::operator=(Texture2D &&other) {
    m_TextureId   = other.m_TextureId;
    m_LocalBuffer = other.m_LocalBuffer;
    m_Height      = other.m_Height;
    m_Width       = other.m_Width;
    m_BPP         = other.m_BPP;

    other.m_TextureId   = 0;
    other.m_LocalBuffer = 0;
    other.m_Height      = 0;
    other.m_Width       = 0;
    other.m_BPP         = 0;

    return *this;
}

void Texture2D::AllocateTexture() {
    gapi.TexImage2D(TEXTURE_TYPE::_2D, 0, INTERNAL_FORMAT::RGBA, m_Width, m_Height, 0, INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, nullptr);
}

void Texture2D::LoadImage(uint32_t width, uint32_t height, std::shared_ptr<unsigned char>& image) {
    gapi.BindTexture(TEXTURE_TYPE::_2D, m_TextureId);

    if (width != m_Width || height != m_Height || !m_LocalBuffer) {
        m_Width = width;
        m_Height = height;
        AllocateTexture();
    }

    m_LocalBuffer = image;

    gapi.TexSubImage2D(TEXTURE_TYPE::_2D, 0, 0, 0, (int) m_Width, (int) m_Height, INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, m_LocalBuffer.get());
}

void Texture2D::Parameteri(GAPI::TEXTURE_PROPERTY texProp, GAPI::TEXTURE_PARAM texParam) {
    gapi.TexParameteri(TEXTURE_TYPE::_2D, texProp, texParam);
}

void Texture2D::Bind(uint32_t slot = 0) const {
    gapi.ActiveTexture(0);
    gapi.BindTexture(TEXTURE_TYPE::_2D, m_TextureId);
}

void Texture2D::UnBind() const {
    gapi.BindTexture(TEXTURE_TYPE::_2D, 0);
}

Texture2D::~Texture2D() {
    gapi.DeleteTextures(1, &m_TextureId);
}

void Texture2D::SetWidth(uint32_t width) { m_Width = width; }
void Texture2D::SetHeight(uint32_t height) { m_Height = height; }
uint32_t Texture2D::GetWidth() const { return m_Width; }
uint32_t Texture2D::GetHeight() const { return m_Height; }