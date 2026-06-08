#include <bit>
#include "graphics_api_opengl.h"
#include "Logger/logger.h"

#define GLEW_STATIC
#include "GL/glew.h"

GAPI_NAMESPACE_USING

struct GL {
    static constexpr int BUFFER_TYPE[] = { GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_FRAMEBUFFER, GL_RENDERBUFFER };
    static constexpr int DRAW_TYPE[] = { GL_STATIC_DRAW, GL_DYNAMIC_DRAW };

    static constexpr int RENDERER_TYPE[] = { GL_TRIANGLES, GL_LINES, GL_POINTS };
    static constexpr int CLEAR_BUFFER_BIT[] = { GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT };

    static constexpr int SHADER_TYPE[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER, GL_GEOMETRY_SHADER };
    static constexpr int SHADER_COMPILE[] = { GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH };
    static constexpr int SHADER_PROGRAM_COMPILE[] = { GL_LINK_STATUS, GL_INFO_LOG_LENGTH };

    static constexpr int TEXTURE_TYPE[] = { GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY };
    static constexpr int FORMAT[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_RGBA };
    static constexpr int INTERNAL_FORMAT[] = { GL_RGB8, GL_RGBA8, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, GL_COLOR_ATTACHMENT0 };
    static constexpr int TEXTURE_PROPERTY[] = { GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T };
    static constexpr int TEXTURE_PARAM[] = { GL_NEAREST, GL_LINEAR, GL_CLAMP_TO_EDGE };

    static constexpr int BLEND_PARAM[] = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };

    static GLenum ConvertAPITypeToGlType(API_TYPE type);
};


int GAPI_OpenGL::Init() {
    return (int) glewInit();
}

void GAPI_OpenGL::CreateBuffer(uint32_t* id) {
    glGenBuffers(1, id);
}

void GAPI_OpenGL::BufferData(BUFFER_TYPE bufferType, uint32_t capacity, const void* data, DRAW_TYPE drawType) {
    int bf_index = static_cast<int>(bufferType);
    int dt_index = static_cast<int>(drawType);

    glBufferData(GL::BUFFER_TYPE[bf_index], capacity, data, GL::DRAW_TYPE[dt_index]);
}

void GAPI_OpenGL::BufferSubData(BUFFER_TYPE bufferType, ptrdiff_t offset, uint32_t size, const void* data) {
    int bf_index = static_cast<int>(bufferType);
    glBufferSubData(GL::BUFFER_TYPE[bf_index], offset, size, data);
}

void GAPI_OpenGL::BindBuffer(BUFFER_TYPE bufferType, uint32_t id) {
    int bf_index = static_cast<int>(bufferType);
    glBindBuffer(GL::BUFFER_TYPE[bf_index], id);
}

void GAPI_OpenGL::DeleteBuffer(uint32_t* id) {
    glDeleteBuffers(1, id);
    *id = 0;
}

void GAPI_OpenGL::CreateVertexArray(uint32_t* id) {
    glGenVertexArrays(1, id);
}

void GAPI_OpenGL::BindVertexArray(uint32_t id) {
    glBindVertexArray(id);
}

void GAPI_OpenGL::VertexAttribPointer(uint32_t index, uint32_t size, API_TYPE type, API_BOOLEAN normalized,
                                          uint32_t stride, const void* pointer)
{
    glVertexAttribPointer(index, (int) size, GL::ConvertAPITypeToGlType(type), static_cast<int>(normalized), (int) stride, pointer);
}

void GAPI_OpenGL::EnableVertexAttribArray(uint32_t location) {
    glEnableVertexAttribArray(location);
}

void GAPI_OpenGL::DeleteVertexArray(uint32_t* id) {
    glDeleteVertexArrays(1, id);
    *id = 0;
}

uint32_t GAPI_OpenGL::CreateShader(SHADER_TYPE type) {
    GLenum gl_stype = GL::SHADER_TYPE[std::countr_zero(static_cast<uint32_t>(type))];
    return glCreateShader(gl_stype);
}

void GAPI_OpenGL::ShaderSource(uint32_t id, uint32_t count, const char** src, const int* length) {
    glShaderSource(id, count, src, length);
}

void GAPI_OpenGL::CompileShader(uint32_t id) {
    glCompileShader(id);
}

void GAPI_OpenGL::GetShaderiv(uint32_t shader, SHADER_COMPILE shaderCompile, int* length) {
    GLenum callToIv = GL::SHADER_COMPILE[static_cast<int>(shaderCompile)];
    glGetShaderiv(shader, callToIv, length);
}

void GAPI_OpenGL::GetShaderInfoLog(uint32_t shader, int bufferSize, int* length, char* msg) {
    glGetShaderInfoLog(shader, bufferSize, length, msg);
}

void GAPI_OpenGL::DeleteShader(uint32_t* id) {
    glDeleteShader(*id);
    *id = 0;
}

uint32_t GAPI_OpenGL::CreateProgram() {
    return glCreateProgram();
}

void GAPI_OpenGL::AttachShader(uint32_t program, uint32_t shader) {
    glAttachShader(program, shader);
}

void GAPI_OpenGL::LinkProgram(uint32_t program) {
    glLinkProgram(program);
}

void GAPI_OpenGL::GetProgramiv(uint32_t program, SHADER_PROGRAM_COMPILE spc, int* length) {
    GLenum a = GL::SHADER_PROGRAM_COMPILE[static_cast<int>(spc)];
    glGetProgramiv(program, a, length);
}

void GAPI_OpenGL::GetProgramInfoLog(uint32_t program, int bufferSize, int* length, char* msg) {
    glGetProgramInfoLog(program, bufferSize, length, msg);
}

void GAPI_OpenGL::UseProgram(uint32_t program) {
    glUseProgram(program);
}

void GAPI_OpenGL::DeleteProgram(uint32_t* program) {
    glDeleteProgram(*program);
    *program = 0;
}

int GAPI_OpenGL::GetUniformLocation(uint32_t program, const char* uniformName) {
    return glGetUniformLocation(program, uniformName);
}

void GAPI_OpenGL::Uniform1i(int location, int value) {
    glUniform1i(location, value);
}

void GAPI_OpenGL::Uniform1f(int location, float value) {
    glUniform1f(location, value);
}

void GAPI_OpenGL::Uniform1iv(int location, uint32_t count, const int* value) {
    glUniform1iv(location, count, value);
}

void GAPI_OpenGL::UniformMatrix4fv(int location, uint32_t count, API_BOOLEAN transpose,
                                       const float* matrix) {
    glUniformMatrix4fv(location, count, static_cast<GLboolean>(transpose), matrix);
}

void GAPI_OpenGL::CreateTextures(uint32_t count, uint32_t* textureId) {
    glGenTextures(count, textureId);
}

void GAPI_OpenGL::BindTexture(TEXTURE_TYPE tex, uint32_t textureId) {
    glBindTexture(GL::TEXTURE_TYPE[static_cast<int>(tex)], textureId);
}

void GAPI_OpenGL::TexImage2D(TEXTURE_TYPE tex, int level, INTERNAL_FORMAT int_form,
                                 uint32_t width, uint32_t height, int border, FORMAT format,
                                 API_TYPE type, const void* data)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLint gl_int_form = GL::INTERNAL_FORMAT[static_cast<int>(int_form)];
    GLenum gl_format = GL::FORMAT[static_cast<int>(format)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glTexImage2D(gl_tex_type, level, gl_int_form, (int) width, (int) height, border, gl_format, gl_type, data);
}

void GAPI_OpenGL::TexSubImage2D(TEXTURE_TYPE tex, int level, uint32_t xOffset, uint32_t yOffset,
                                    uint32_t width, uint32_t height, FORMAT format,
                                    API_TYPE type, const void* pixels)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_format = GL::FORMAT[static_cast<int>(format)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glTexSubImage2D(gl_tex_type, level, (int) xOffset, (int) yOffset, (int) width, (int) height, gl_format, gl_type, pixels);
}

void GAPI_OpenGL::TexImage3D(TEXTURE_TYPE tex, int level, INTERNAL_FORMAT int_form,
                                 uint32_t width, uint32_t height, uint32_t layerCount, uint32_t border,
                                 FORMAT format, API_TYPE type, const void* data)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_int_form = GL::INTERNAL_FORMAT[static_cast<int>(int_form)];
    GLenum gl_format = GL::FORMAT[static_cast<int>(format)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glTexImage3D(gl_tex_type, level, gl_int_form, (int) width, (int) height, (int) layerCount, (int) border, gl_format, gl_type, data);
}

void GAPI_OpenGL::TexSubImage3D(TEXTURE_TYPE tex, int level, int xOffset, int yOffset, int zOffset,
                                    uint32_t width, uint32_t height, int depth, FORMAT format,
                                    API_TYPE type, const void* data)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_format = GL::FORMAT[static_cast<int>(format)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glTexSubImage3D(gl_tex_type, level, xOffset, yOffset, zOffset, (int) width, (int) height, (int) depth, gl_format, gl_type, data);
}

void GAPI_OpenGL::ActiveTexture(uint32_t slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
}

void GAPI_OpenGL::DeleteTextures(uint32_t count, uint32_t* textureId) {
    glDeleteTextures(count, textureId);
    *textureId = 0;
}

void GAPI_OpenGL::TexParameteri(TEXTURE_TYPE tex, TEXTURE_PROPERTY texProp, TEXTURE_PARAM texParam) {
    GLenum gl_tex = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_tex_prop = GL::TEXTURE_PROPERTY[static_cast<int>(texProp)];
    GLenum gl_tex_param = GL::TEXTURE_PARAM[static_cast<int>(texParam)];

    glTexParameteri(gl_tex, gl_tex_prop, gl_tex_param);
}

void GAPI_OpenGL::DrawElements(RENDERER_TYPE rt, uint32_t count, API_TYPE type, const void* indices) {
    GLenum renderer_type = GL::RENDERER_TYPE[static_cast<int>(rt)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glDrawElements(renderer_type, (int) count, gl_type, indices);
}

std::string GAPI_OpenGL::GetApiVersion() {
    return (const char*)glGetString(GL_VERSION);
}

void GAPI_OpenGL::Clear(CLEAR_BUFFER_BIT bits) {
    GLbitfield result = 0;

    if ((static_cast<int>(bits) & static_cast<uint32_t>(CLEAR_BUFFER_BIT::COLOR)) != 0) {
        result |= GL_COLOR_BUFFER_BIT;
    }
    if ((static_cast<int>(bits) & static_cast<uint32_t>(CLEAR_BUFFER_BIT::DEPTH)) != 0) {
        result |= GL_DEPTH_BUFFER_BIT;
    }
    if ((static_cast<int>(bits) & static_cast<uint32_t>(CLEAR_BUFFER_BIT::STENCIL)) != 0) {
        result |= GL_STENCIL_BUFFER_BIT;
    }

    glClear(result);
}

void GAPI_OpenGL::EnableBlending() {
    glEnable(GL_BLEND);
}

void GAPI_OpenGL::BlendFunc(BLEND_PARAM param1, BLEND_PARAM param2) {
    GLenum gl_p1 = GL::BLEND_PARAM[static_cast<int>(param1)];
    GLenum gl_p2 = GL::BLEND_PARAM[static_cast<int>(param2)];

    glBlendFunc(gl_p1, gl_p2);
}

std::string GAPI_OpenGL::GetShaderLanguageVersion(IGraphicsAPIContext* context) {
    std::string result = "#version ";
    result.append(std::to_string(context->GetMajorV()));
    result.append(std::to_string(context->GetMinorV()));
    result.append(std::to_string(0) + " core");
    return result;
}

GAPI_OpenGL& GAPI_OpenGL::Get()  {
    static GAPI_OpenGL graphicsApi;
    return graphicsApi;
}

uint32_t GAPI_OpenGL::GetMaxArrayTexLayers() {
    int layerCount;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &layerCount);
    return (int) layerCount;
}

void GAPI_OpenGL::CreateFramebuffer(uint32_t* id) {
    glGenFramebuffers(1, id);
}

void GAPI_OpenGL::BindFramebuffer(uint32_t id) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void GAPI_OpenGL::DeleteFramebuffer(uint32_t* id) {
    glDeleteFramebuffers(1, id);
}

void GAPI_OpenGL::CreateRenderbuffer(uint32_t* id) {
    glGenRenderbuffers(1, id);
}

void GAPI_OpenGL::BindRenderbuffer(uint32_t id) {
    glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void GAPI_OpenGL::RenderbufferStorage(INTERNAL_FORMAT depthStencil, uint32_t width, uint32_t height) {
    glRenderbufferStorage(GL_RENDERBUFFER, GL::INTERNAL_FORMAT[static_cast<int>(depthStencil)], (int) width, (int) height);
}

void GAPI_OpenGL::DeleteRenderbuffer(uint32_t* id) {
    glDeleteRenderbuffers(1, id);
}

void GAPI_OpenGL::FramebufferRenderbuffer(BUFFER_TYPE target, INTERNAL_FORMAT internalFormat, BUFFER_TYPE renderBufferTarget, uint32_t renderBufId) {
    glFramebufferRenderbuffer(GL::BUFFER_TYPE[static_cast<int>(target)],
                              GL::INTERNAL_FORMAT[static_cast<int>(internalFormat)],
                              GL::BUFFER_TYPE[static_cast<int>(renderBufferTarget)],
                              renderBufId);
}

void GAPI_OpenGL::FramebufferTexture(BUFFER_TYPE target, INTERNAL_FORMAT attachment, uint32_t texId, uint32_t level) {
    glFramebufferTexture(GL::BUFFER_TYPE[static_cast<int>(target)],
                         GL::INTERNAL_FORMAT[static_cast<int>(attachment)],
                         texId,
                         0);
}

void GAPI_OpenGL::FramebufferTextureLayer(BUFFER_TYPE target, INTERNAL_FORMAT attachment, uint32_t texId, uint32_t level,
                                              uint32_t layer) {
    glFramebufferTextureLayer(GL::BUFFER_TYPE[static_cast<int>(target)],
                              GL::INTERNAL_FORMAT[static_cast<int>(attachment)],
                              texId,
                              0,
                              layer);
}

void GAPI_OpenGL::Viewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}

void GAPI_OpenGL::EnableDepthTest() {
    glEnable(GL_DEPTH_TEST);
}

GLenum GL::ConvertAPITypeToGlType(API_TYPE type) {
    switch (type) {
        case API_TYPE::FLOAT: return GL_FLOAT;
        case API_TYPE::INT:   return GL_INT;
        case API_TYPE::UINT:  return GL_UNSIGNED_INT;
        case API_TYPE::UCHAR: return GL_UNSIGNED_BYTE;
        case API_TYPE::BOOL:  return GL_BOOL;
    }

    return 0;
}
