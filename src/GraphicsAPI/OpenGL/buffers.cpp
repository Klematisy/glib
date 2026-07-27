#include "GraphicsAPI/graphics_api.h"
#include "graphics_api_opengl.h"

#include "type_casting.h"

#include "logger.h"

VertexBufferOpenGL::VertexBufferOpenGL(GAPI::DRAW_TYPE drawType, u32 size, const u0* data)
    : VertexBuffer()
{
    m_DrawType = drawType;
    m_Capacity = size;

    glGenBuffers(1, &m_ID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, m_Capacity, nullptr, GL::DRAW_TYPE[TO_INT(m_DrawType)]);
    UnBind();
}

VertexBufferOpenGL::~VertexBufferOpenGL() {
    if (m_ID != 0)
        glDeleteBuffers(1, &m_ID);
}

VertexBufferOpenGL& VertexBufferOpenGL::operator=(VertexBufferOpenGL&& other) {
    m_Capacity   = other.m_Capacity;
    m_ID         = other.m_ID;
    m_DrawType = other.m_DrawType;

    other.m_Capacity = 0;
    other.m_ID       = 0;

    return *this;
}


u0 VertexBufferOpenGL::PutData(const u0* data, u32 size) {
    if (m_DrawType == DRAW_TYPE::STATIC) {
        LOGERR("The vertex buffer is static!");
        return;
    }

    if (!data) return;

    Bind();

    if (m_Capacity < size) {
        m_Capacity = size;
        glBufferData(GL_ARRAY_BUFFER, m_Capacity, data, GL::DRAW_TYPE[TO_INT(m_DrawType)]);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    UnBind();
}

u0 VertexBufferOpenGL::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

u0 VertexBufferOpenGL::UnBind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


ElementBufferOpenGL::ElementBufferOpenGL(GAPI::DRAW_TYPE drawType, u32 count, const u0* data)
    : ElementBuffer()
{
    m_DrawType = drawType;
    m_Count = count;

    glGenBuffers(1, &m_ID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), data, GL::DRAW_TYPE[TO_INT(m_DrawType)]);
    UnBind();
}

ElementBufferOpenGL::~ElementBufferOpenGL() {
    if (m_ID != 0)
        glDeleteBuffers(1, &m_ID);
}

ElementBufferOpenGL& ElementBufferOpenGL::operator=(ElementBufferOpenGL&& other) {
    m_DrawType = other.m_DrawType;
    m_ID         = other.m_ID;
    m_Count      = other.m_Count;

    other.m_ID       = 0;
    other.m_Count    = 0;

    return *this;
}

u0 ElementBufferOpenGL::PutData(const u0* data, u32 count) {
    if (m_DrawType == DRAW_TYPE::STATIC) {
        LOGERR("The element buffer is static!");
        return;
    }

    Bind();

    if (m_Count < count) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL::DRAW_TYPE[TO_INT(m_DrawType)]);
    } else {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), data);
    }

    m_Count = count;
    UnBind();
}

u0 ElementBufferOpenGL::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

u0 ElementBufferOpenGL::UnBind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

u32 ElementBufferOpenGL::GetCount() const {
    return m_Count;
}

VertexArrayOpenGL::VertexArrayOpenGL() {
    glGenVertexArrays(1, &m_ID);
}

VertexArrayOpenGL::~VertexArrayOpenGL() {
    glDeleteVertexArrays(1, &m_ID);
}

VertexArrayOpenGL& VertexArrayOpenGL::operator=(VertexArrayOpenGL&& other) {
    m_ID = other.m_ID;
    other.m_ID = 0;

    return *this;
}

u0 VertexArrayOpenGL::Bind() const {
    glBindVertexArray(m_ID);
}

u0 VertexArrayOpenGL::UnBind() const {
    glBindVertexArray(0);
}

u0 VertexArrayOpenGL::AddElementBuffer(const ElementBuffer& vb) {
    Bind();
    vb.Bind();

    UnBind();
    vb.UnBind();
}

u0 VertexArrayOpenGL::AddVertexBuffer(const VertexArrayLayout& layout, const VertexBuffer& vb) {
    const auto& layouts = layout.GetLayouts();

    Bind();
    vb.Bind();

    int fullOffset = layout.GetFullOffset();

    u32 i = 0;
    for (const LayoutData &element : layouts) {

        glVertexAttribPointer(i, (int) element.size, GL::ConvertAPITypeToGlType(element.type), GL_FALSE,  fullOffset, (u0*) element.offset);
        glEnableVertexAttribArray(i++);
    }

    UnBind();
    vb.UnBind();
}



FramebufferOpenGL::FramebufferOpenGL() {
    glGenFramebuffers(1, &m_FBO);
}

FramebufferOpenGL::~FramebufferOpenGL() {
    glDeleteFramebuffers(1, &m_FBO);
}

u0 FramebufferOpenGL::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

u0 FramebufferOpenGL::UnBind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



RenderbufferOpenGL::RenderbufferOpenGL() {
    glGenRenderbuffers(1, &m_RB);
}

RenderbufferOpenGL::~RenderbufferOpenGL() {
    glDeleteRenderbuffers(1, &m_RB);
}

u0 RenderbufferOpenGL::Init(u32 width, u32 height, GAPI::INTERNAL_FORMAT depthStencil) {
    Bind();
    r_Width = width;
    r_Height = height;
    glRenderbufferStorage(GL_RENDERBUFFER, GL::INTERNAL_FORMAT[TO_INT(depthStencil)], r_Width, r_Height);
    UnBind();
}

u0 RenderbufferOpenGL::Bind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, m_RB);
}

u0 RenderbufferOpenGL::UnBind() const {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}


u0 GAPI::AttachFramebufferToRenderbuffer(const Framebuffer& fb, const Renderbuffer& rb, INTERNAL_FORMAT depthStencil) {
    fb.Bind();
    rb.Bind();

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL::INTERNAL_FORMAT[TO_INT(depthStencil)], GL_RENDERBUFFER, rb.m_RB);

    rb.UnBind();
    fb.UnBind();
}

u0 GAPI::AttachTextureToFramebuffer(const Framebuffer& fb, const Texture2D& tex, INTERNAL_FORMAT attachment) {
    fb.Bind();
    tex.Bind(0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL::INTERNAL_FORMAT[TO_INT(attachment)], tex.m_ID, 0);

    tex.UnBind();
    fb.UnBind();
}

u0 GAPI::AttachTextureArrayToFramebuffer(const Framebuffer& fb, const TextureArray& tex, INTERNAL_FORMAT attachment, u32 layer) {
    fb.Bind();
    tex.Bind();

    glFramebufferTextureLayer(GL_FRAMEBUFFER, GL::INTERNAL_FORMAT[TO_INT(attachment)], tex.m_ID, 0, layer);

    tex.UnBind();
    fb.UnBind();
}
