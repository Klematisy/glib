#include "graphics_api_opengl.h"

#include "logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "type_casting.h"

ImageInfo::ImageInfo(u32 w, u32 h, u32 bpp, std::shared_ptr<u8> bitmap)
    : m_W(w), m_H(h), m_BPP(bpp), m_Bitmap(std::move(bitmap))
{}

ImageInfo::ImageInfo(const char* fileName) {
    m_Bitmap = std::shared_ptr<u8>(
        stbi_load(fileName, (i32*)&m_W, (i32*)&m_H, (i32*)&m_BPP, 4),
        [](u8* p) {
            stbi_image_free(p);
        }
    );
    if (!m_Bitmap) {
        using namespace std::string_literals;
        LOGERR("STBi: Image '"s + fileName + "' hasn't loaded!");
    }
}

u32 ImageInfo::GetWidth() const { return m_W; }
u32 ImageInfo::GetHeight() const { return m_H; }
u32 ImageInfo::GetBPP() const { return m_BPP; }
std::shared_ptr<u8> ImageInfo::GetBitmap() const { return m_Bitmap; }
const TextureParameters& ImageInfo::GetTexParams() const { return m_TP; }

void ImageInfo::SetTexParam(const TextureParameters& texParam) { m_TP = texParam; }


static const i32 stbi_init = []() {
    stbi_set_flip_vertically_on_load(1);
    return 0;
}();

Texture2DOpenGL::Texture2DOpenGL()
    : Texture2D()
{
    glGenTextures(1, &m_ID);
}

Texture2DOpenGL::Texture2DOpenGL(Texture2DOpenGL&& other)
    : Texture2D()
{
    m_ID = other.m_ID;
    other.m_ID = 0;
}

Texture2DOpenGL::~Texture2DOpenGL() {
    glDeleteTextures(1, &m_ID);
}

Texture2DOpenGL& Texture2DOpenGL::operator=(Texture2DOpenGL&& other) {
    m_ID = other.m_ID;
    other.m_ID = 0;

    return *this;
}

void Texture2DOpenGL::Bind(u32 slot) const {
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture2DOpenGL::UnBind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2DOpenGL::Upload(const ImageInfo &info) {
    Bind();

    auto& tp = m_TexParameters;
    if (tp != info.GetTexParams()) {
        tp = info.GetTexParams();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL::TEXTURE_PARAM[TO_INT(tp.magFilter)]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL::TEXTURE_PARAM[TO_INT(tp.minFilter)]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL::TEXTURE_PARAM[TO_INT(tp.wrapS)]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL::TEXTURE_PARAM[TO_INT(tp.wrapT)]);
    }

    if (m_AllocatedW != info.GetWidth() || m_AllocatedH != info.GetHeight()) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL::INTERNAL_FORMAT[TO_INT(INTERNAL_FORMAT::RGBA8)], info.GetWidth(), info.GetHeight(), 0, GL::FORMAT[TO_INT(FORMAT::RGBA)], GL::ConvertAPITypeToGlType(API_TYPE::UCHAR), info.GetBitmap().get());
        m_AllocatedW = info.GetWidth();
        m_AllocatedH = info.GetHeight();
    } else
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, info.GetWidth(), info.GetHeight(), GL::FORMAT[TO_INT(FORMAT::RGBA)], GL::ConvertAPITypeToGlType(API_TYPE::UCHAR), info.GetBitmap().get());

    m_W = info.GetWidth();
    m_H = info.GetHeight();

    UnBind();
}

const TextureParameters& Texture2DOpenGL::GetTexParameters() const { return m_TexParameters; }
u32 Texture2DOpenGL::GetWidth() const { return m_W; }
u32 Texture2DOpenGL::GetHeight() const { return m_H; }



TextureArrayOpenGL::TextureArrayOpenGL()
    : TextureArray()
{
    glGenTextures(1, &m_ID);
}

TextureArrayOpenGL::TextureArrayOpenGL(TextureArrayOpenGL&& other) noexcept
    : TextureArray()
{
    m_W = other.m_W;
    m_H = other.m_H;
    m_Layers = other.m_Layers;
    m_ID = other.m_ID;

    other.m_W = 0;
    other.m_H = 0;
    other.m_Layers = 0;
    other.m_ID = 0;
}

TextureArrayOpenGL& TextureArrayOpenGL::operator=(TextureArrayOpenGL&& other) {
    m_W = other.m_W;
    m_H = other.m_H;
    m_Layers = other.m_Layers;
    m_ID = other.m_ID;

    other.m_W = 0;
    other.m_H = 0;
    other.m_Layers = 0;
    other.m_ID = 0;

    return *this;
}

TextureArrayOpenGL::~TextureArrayOpenGL() {
    glDeleteTextures(1, &m_ID);
}

void TextureArrayOpenGL::Init(u32 width, u32 height, u32 layers, const TextureParameters& tp) {
    Bind();

    m_W = width;
    m_H = height;
    m_Layers = layers;
    m_TexParameters = tp;

    i32 maxLayers = 0;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);
    if (layers > maxLayers) {
        m_Layers = maxLayers;
        LOGWARN("TEXTURE ARRAY: You specified layers count more than your PC support. "
                       "That's why layer count will be equal max layer count on your PC");
        return;
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL::TEXTURE_PARAM[TO_INT(tp.magFilter)]);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL::TEXTURE_PARAM[TO_INT(tp.minFilter)]);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL::TEXTURE_PARAM[TO_INT(tp.wrapS)]);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL::TEXTURE_PARAM[TO_INT(tp.wrapT)]);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL::INTERNAL_FORMAT[TO_INT(INTERNAL_FORMAT::RGBA8)],
                    (i32) m_W, (i32) m_H, (i32) m_Layers,
                    0, GL::FORMAT[TO_INT(FORMAT::RGBA)], GL::ConvertAPITypeToGlType(API_TYPE::UCHAR), nullptr);
    UnBind();
}

void TextureArrayOpenGL::AddImage(const ImageInfo& info, u32 xOffset, u32 yOffset, u32 slot)
{
    if (slot >= m_Layers) {
        LOGERR("TEXTURE ARRAY OPENGL: Slot index out of range!");
        return;
    }

    if (m_W < info.GetWidth() || m_H < info.GetHeight()) {
        LOGERR("TEXTURE ARRAY OPENGL: Texture out of texture array range!");
        return;
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                    (i32) xOffset, (i32) yOffset, (i32) slot,
                    (i32) info.GetWidth(), (i32) info.GetHeight(), 1,
                    GL::FORMAT[TO_INT(FORMAT::RGBA)], GL::ConvertAPITypeToGlType(API_TYPE::UCHAR), info.GetBitmap().get());
}

void TextureArrayOpenGL::Bind(u32 slot) const {
    if (m_ID == 0) {
        LOGERR("TextureArrayOpenGL is empty!");
        return;
    }
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);
}

void TextureArrayOpenGL::UnBind() const {
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

u32 TextureArrayOpenGL::GetWidth() const { return m_W; }
u32 TextureArrayOpenGL::GetHeight() const { return m_H; }
u32 TextureArrayOpenGL::GetLayersCount() const { return m_Layers; }
const TextureParameters& TextureArrayOpenGL::GetTexParameters() const { return m_TexParameters; }
