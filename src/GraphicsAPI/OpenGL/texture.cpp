#include "graphics_api_opengl.h"

#include "logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "type_casting.h"

ImageInfo::ImageInfo(u32 w, u32 h, u32 bpp, std::shared_ptr<u8> bitmap)
    : r_Width(w), r_Height(h), r_BPP(bpp), r_Bitmap(std::move(bitmap))
{}

ImageInfo::ImageInfo(const char* fileName) {
    r_Bitmap = std::shared_ptr<u8>(
        stbi_load(fileName, (i32*)&r_Width, (i32*)&r_Height, (i32*)&r_BPP, 4),
        [](u8* p) {
            stbi_image_free(p);
        }
    );
    if (!r_Bitmap) {
        using namespace std::string_literals;
        LOGERR("STBi: Image '"s + fileName + "' hasn't loaded!");
    }
}

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

    auto& tp = r_TexParameters;
    if (tp != info.texParams) {
        tp = info.texParams;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL::TEXTURE_PARAM[TO_INT(tp.magFilter)]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL::TEXTURE_PARAM[TO_INT(tp.minFilter)]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL::TEXTURE_PARAM[TO_INT(tp.wrapS)]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL::TEXTURE_PARAM[TO_INT(tp.wrapT)]);
    }

    if (m_AllocatedW != info.r_Width || m_AllocatedH != info.r_Height) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL::INTERNAL_FORMAT[TO_INT(INTERNAL_FORMAT::RGBA8)], info.r_Width, info.r_Height, 0, GL::FORMAT[TO_INT(FORMAT::RGBA)], GL::ConvertAPITypeToGlType(API_TYPE::UCHAR), info.r_Bitmap.get());
        m_AllocatedW = info.r_Width;
        m_AllocatedH = info.r_Height;
    } else
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, info.r_Width, info.r_Height, GL::FORMAT[TO_INT(FORMAT::RGBA)], GL::ConvertAPITypeToGlType(API_TYPE::UCHAR), info.r_Bitmap.get());

    r_Width = info.r_Width;
    r_Height = info.r_Height;

    UnBind();
}


TextureArrayOpenGL::TextureArrayOpenGL()
    : TextureArray()
{
    glGenTextures(1, &m_ID);
}

TextureArrayOpenGL::TextureArrayOpenGL(TextureArrayOpenGL&& other) noexcept
    : TextureArray()
{
    r_Width = other.r_Width;
    r_Height = other.r_Height;
    r_Layers = other.r_Layers;
    m_ID = other.m_ID;

    other.r_Width = 0;
    other.r_Height = 0;
    other.r_Layers = 0;
    other.m_ID = 0;
}

TextureArrayOpenGL& TextureArrayOpenGL::operator=(TextureArrayOpenGL&& other) {
    r_Width = other.r_Width;
    r_Height = other.r_Height;
    r_Layers = other.r_Layers;
    m_ID = other.m_ID;

    other.r_Width = 0;
    other.r_Height = 0;
    other.r_Layers = 0;
    other.m_ID = 0;

    return *this;
}

TextureArrayOpenGL::~TextureArrayOpenGL() {
    glDeleteTextures(1, &m_ID);
}

void TextureArrayOpenGL::Init(u32 width, u32 height, u32 layers, const TextureParameters& tp) {
    Bind();

    r_Width = width;
    r_Height = height;
    r_Layers = layers;
    r_TexParameters = tp;

    i32 maxLayers = 0;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxLayers);
    if (layers > maxLayers) {
        r_Layers = maxLayers;
        LOGWARN("TEXTURE ARRAY: You specified layers count more than your PC support. "
                       "That's why layer count will be equal max layer count on your PC");
        return;
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL::TEXTURE_PARAM[TO_INT(tp.magFilter)]);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL::TEXTURE_PARAM[TO_INT(tp.minFilter)]);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL::TEXTURE_PARAM[TO_INT(tp.wrapS)]);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL::TEXTURE_PARAM[TO_INT(tp.wrapT)]);

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL::INTERNAL_FORMAT[TO_INT(INTERNAL_FORMAT::RGBA8)],
                    (i32) r_Width, (i32) r_Height, (i32) r_Layers,
                    0, GL::FORMAT[TO_INT(FORMAT::RGBA)], GL::ConvertAPITypeToGlType(API_TYPE::UCHAR), nullptr);
    UnBind();
}

void TextureArrayOpenGL::AddImage(const ImageInfo& info, u32 xOffset, u32 yOffset, u32 slot)
{
    if (slot >= r_Layers) {
        LOGERR("TEXTURE ARRAY OPENGL: Slot index out of range!");
        return;
    }

    if (r_Width < info.r_Width || r_Height < info.r_Height) {
        LOGERR("TEXTURE ARRAY OPENGL: Texture out of texture array range!");
        return;
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                    (i32) xOffset, (i32) yOffset, (i32) slot,
                    (i32) info.r_Width, (i32) info.r_Height, 1,
                    GL::FORMAT[TO_INT(FORMAT::RGBA)], GL::ConvertAPITypeToGlType(API_TYPE::UCHAR), info.r_Bitmap.get());
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
