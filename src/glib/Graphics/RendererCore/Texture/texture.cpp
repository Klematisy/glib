#include <iostream>

#include "../renderer.h"
#include "texture.h"

using namespace RendererCore;
using namespace GAPI;

static GraphicsAPIImpl& gapi = GraphicsAPIImpl::Get();

Texture::Texture() {
    gapi.CreateTextures(1, &m_TextureID);
    gapi.BindTexture(TEXTURE_TYPE::_2D, m_TextureID);

    gapi.TexParameteri(TEXTURE_TYPE::_2D, TEXTURE_PROPERTY::MIN_FILTER, TEXTURE_PARAM::LINEAR);
    gapi.TexParameteri(TEXTURE_TYPE::_2D, TEXTURE_PROPERTY::MAG_FILTER, TEXTURE_PARAM::LINEAR);

    gapi.TexParameteri(TEXTURE_TYPE::_2D, TEXTURE_PROPERTY::WRAP_S, TEXTURE_PARAM::CLAMP_TO_EDGE);
    gapi.TexParameteri(TEXTURE_TYPE::_2D, TEXTURE_PROPERTY::WRAP_T, TEXTURE_PARAM::CLAMP_TO_EDGE);

    uint32_t whitePixel = 0xFFFFFFFF;
    gapi.TexImage2D(TEXTURE_TYPE::_2D, 0, INTERNAL_FORMAT::RGB8, 1, 1, 0, INTERNAL_FORMAT::RGBA8, API_TYPE::UCHAR, &whitePixel);

    UnBind();
}

Texture::Texture(Texture &&other) {
    m_TextureID   = other.m_TextureID;
    m_LocalBuffer = other.m_LocalBuffer;
    m_Height      = other.m_Height;
    m_Width       = other.m_Width;
    m_BPP         = other.m_BPP;

    other.m_TextureID   = 0;
    other.m_LocalBuffer = 0;
    other.m_Height      = 0;
    other.m_Width       = 0;
    other.m_BPP         = 0;
}

Texture& Texture::operator=(Texture &&other) {
    m_TextureID   = other.m_TextureID;
    m_LocalBuffer = other.m_LocalBuffer;
    m_Height      = other.m_Height;
    m_Width       = other.m_Width;
    m_BPP         = other.m_BPP;

    other.m_TextureID   = 0;
    other.m_LocalBuffer = 0;
    other.m_Height      = 0;
    other.m_Width       = 0;
    other.m_BPP         = 0;

    return *this;
}

void Texture::LoadImage(const char *filePath) {

    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(filePath, &m_Width, &m_Height, &m_BPP, 4);

    gapi.BindTexture(TEXTURE_TYPE::_2D, m_TextureID);
    gapi.TexImage2D(TEXTURE_TYPE::_2D, 0, INTERNAL_FORMAT::RGBA8, m_Width, m_Height, 0, INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, m_LocalBuffer);

    using namespace std::string_literals;

    if (m_LocalBuffer) {
        stbi_image_free(m_LocalBuffer);
        Logger::LogInf("TEXTURE", "'"s + filePath + "' has loaded");
    } else {
        Logger::LogErr("TEXTURE", "'"s + filePath + "' hasn't loaded");
    }
}

void Texture::LoadImage(uint32_t width, uint32_t height, unsigned char* image) {
    if (m_LocalBuffer) {
        m_LocalBuffer = image;
        if (width * height > m_Width * m_Height) {
            gapi.BindTexture(TEXTURE_TYPE::_2D, m_TextureID);
            gapi.TexImage2D(TEXTURE_TYPE::_2D, 0, INTERNAL_FORMAT::RGBA, m_Width, m_Height, 0, INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, m_LocalBuffer);
        } else {
            gapi.BindTexture(TEXTURE_TYPE::_2D, m_TextureID);
            gapi.TexSubImage2D(TEXTURE_TYPE::_2D, 0, 0, 0, (int) width, (int) height, INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, m_LocalBuffer);
        }
    } else {
        m_LocalBuffer = image;
        m_Width = width;
        m_Height = height;

        gapi.BindTexture(TEXTURE_TYPE::_2D, m_TextureID);
        gapi.TexImage2D(TEXTURE_TYPE::_2D, 0, INTERNAL_FORMAT::RGBA, m_Width, m_Height, 0, INTERNAL_FORMAT::RGBA, API_TYPE::UCHAR, m_LocalBuffer);
    }
}

void Texture::Bind(uint32_t slot = 0) const {
    gapi.ActiveTexture(0);
    gapi.BindTexture(TEXTURE_TYPE::_2D, m_TextureID);
}

void Texture::UnBind() {
    gapi.BindTexture(TEXTURE_TYPE::_2D, 0);
}

Texture::~Texture() {
    gapi.DeleteTextures(1, &m_TextureID);
}

bool Texture::IsEqual(const Texture &other) const {
    return m_TextureID == other.m_TextureID;
}

int Texture::GetWidth() const {
    return m_Width;
}

int Texture::GetHeight() const {
    return m_Height;
}