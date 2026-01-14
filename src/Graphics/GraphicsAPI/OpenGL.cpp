#include "graphics_api_impl.h"

GAPI_NAMESPACE_USING



int GraphicsAPIImpl::GraphicsInit() {
    return (int) glewInit();
}

void GraphicsAPIImpl::CreateBuffers(uint32_t count, uint32_t* id) {
    glGenBuffers(count, id);
}

void GraphicsAPIImpl::BufferData(BUFFER_TYPE bufferType, uint32_t capacity, const void* data, DRAW_TYPE drawType) {
    int bf_index = static_cast<int>(bufferType);
    int dt_index = static_cast<int>(drawType);

    glBufferData(GL::BUFFER_TYPE[bf_index], capacity, data, GL::DRAW_TYPE[dt_index]);
}

void GraphicsAPIImpl::BufferSubData(BUFFER_TYPE bufferType, ptrdiff_t offset, uint32_t size, const void* data) {
    int bf_index = static_cast<int>(bufferType);
    glBufferSubData(GL::BUFFER_TYPE[bf_index], offset, size, data);
}

void GraphicsAPIImpl::BindBuffer(BUFFER_TYPE bufferType, uint32_t id) {
    int bf_index = static_cast<int>(bufferType);
    glBindBuffer(GL::BUFFER_TYPE[bf_index], id);
}

void GraphicsAPIImpl::DeleteBuffers(uint32_t count, uint32_t* id) {
    glDeleteBuffers(count, id);
    *id = 0;
}

void GraphicsAPIImpl::CreateVertexArrays(uint32_t count, uint32_t* id) {
    glGenVertexArrays(count, id);
}

void GraphicsAPIImpl::BindVertexArray(uint32_t id) {
    glBindVertexArray(id);
}

void GraphicsAPIImpl::VertexAttribPointer(uint32_t index, uint32_t size, API_TYPE type, API_BOOLEAN normalized,
                                          uint32_t stride, const void* pointer)
{
    glVertexAttribPointer(index, (int) size, GL::ConvertAPITypeToGlType(type), static_cast<int>(normalized), (int) stride, pointer);
}

void GraphicsAPIImpl::EnableVertexAttribArray(uint32_t location) {
    glEnableVertexAttribArray(location);
}

void GraphicsAPIImpl::DeleteVertexArrays(uint32_t count, uint32_t* id) {
    glDeleteVertexArrays(count, id);
    *id = 0;
}

uint32_t GraphicsAPIImpl::CreateShader(SHADER_TYPE type) {
    GLenum gl_stype = GL::SHADER_TYPE[static_cast<int>(type)];
    return glCreateShader(gl_stype);
}

void GraphicsAPIImpl::ShaderSource(uint32_t id, uint32_t count, const char** src, const int* length) {
    glShaderSource(id, count, src, length);
}

void GraphicsAPIImpl::CompileShader(uint32_t id) {
    glCompileShader(id);
}

void GraphicsAPIImpl::GetShaderiv(uint32_t shader, SHADER_COMPILE shaderCompile, int* length) {
    GLenum callToIv = GL::SHADER_COMPILE[static_cast<int>(shaderCompile)];
    glGetShaderiv(shader, callToIv, length);
}

void GraphicsAPIImpl::GetShaderInfoLog(uint32_t shader, int bufferSize, int* length, char* msg) {
    glGetShaderInfoLog(shader, bufferSize, length, msg);
}

void GraphicsAPIImpl::DeleteShader(uint32_t* id) {
    glDeleteShader(*id);
    *id = 0;
}

uint32_t GraphicsAPIImpl::CreateProgram() {
    return glCreateProgram();
}

void GraphicsAPIImpl::AttachShader(uint32_t program, uint32_t shader) {
    glAttachShader(program, shader);
}

void GraphicsAPIImpl::LinkProgram(uint32_t program) {
    glLinkProgram(program);
}

void GraphicsAPIImpl::GetProgramiv(uint32_t program, SHADER_PROGRAM_COMPILE spc, int* length) {
    GLenum a = GL::SHADER_PROGRAM_COMPILE[static_cast<int>(spc)];
    glGetProgramiv(program, a, length);
}

void GraphicsAPIImpl::GetProgramInfoLog(uint32_t program, int bufferSize, int* length, char* msg) {
    glGetProgramInfoLog(program, bufferSize, length, msg);
}

void GraphicsAPIImpl::UseProgram(uint32_t program) {
    glUseProgram(program);
}

void GraphicsAPIImpl::DeleteProgram(uint32_t* program) {
    glDeleteProgram(*program);
    *program = 0;
}

int GraphicsAPIImpl::GetUniformLocation(uint32_t program, const char* uniformName) {
    return glGetUniformLocation(program, uniformName);
}

void GraphicsAPIImpl::Uniform1i(int location, int value) {
    glUniform1i(location, value);
}

void GraphicsAPIImpl::Uniform1f(int location, float value) {
    glUniform1f(location, value);
}

void GraphicsAPIImpl::Uniform1iv(int location, uint32_t count, const int* value) {
    glUniform1iv(location, count, value);
}

void GraphicsAPIImpl::UniformMatrix4fv(int location, uint32_t count, API_BOOLEAN transpose,
                                       const float* matrix) {
    glUniformMatrix4fv(location, count, static_cast<GLboolean>(transpose), matrix);
}

void GraphicsAPIImpl::CreateTextures(uint32_t count, uint32_t* textureId) {
    glGenTextures(count, textureId);
}

void GraphicsAPIImpl::BindTexture(TEXTURE_TYPE tex, uint32_t textureId) {
    glBindTexture(GL::TEXTURE_TYPE[static_cast<int>(tex)], textureId);
}

void GraphicsAPIImpl::TexImage2D(TEXTURE_TYPE tex, int level, INTERNAL_FORMAT int_form,
                                 uint32_t width, uint32_t height, int border, FORMAT format,
                                 API_TYPE type, const void* data)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLint gl_int_form = GL::INTERNAL_FORMAT[static_cast<int>(int_form)];
    GLenum gl_format = GL::FORMAT[static_cast<int>(format)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glTexImage2D(gl_tex_type, level, gl_int_form, (int) width, (int) height, border, gl_format, gl_type, data);
}

void GraphicsAPIImpl::TexSubImage2D(TEXTURE_TYPE tex, int level, uint32_t xOffset, uint32_t yOffset,
                                    uint32_t width, uint32_t height, FORMAT format,
                                    API_TYPE type, const void* pixels)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_format = GL::FORMAT[static_cast<int>(format)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glTexSubImage2D(gl_tex_type, level, (int) xOffset, (int) yOffset, (int) width, (int) height, gl_format, gl_type, pixels);
}

void GraphicsAPIImpl::TexImage3D(TEXTURE_TYPE tex, int level, INTERNAL_FORMAT int_form,
                                 uint32_t width, uint32_t height, uint32_t layerCount, uint32_t border,
                                 FORMAT format, API_TYPE type, const void* data)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_int_form = GL::INTERNAL_FORMAT[static_cast<int>(int_form)];
    GLenum gl_format = GL::FORMAT[static_cast<int>(format)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glTexImage3D(gl_tex_type, level, gl_int_form, (int) width, (int) height, (int) layerCount, (int) border, gl_format, gl_type, data);
}

void GraphicsAPIImpl::TexSubImage3D(TEXTURE_TYPE tex, int level, int xOffset, int yOffset, int zOffset,
                                    uint32_t width, uint32_t height, int depth, FORMAT format,
                                    API_TYPE type, const void* data)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_format = GL::FORMAT[static_cast<int>(format)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glTexSubImage3D(gl_tex_type, level, xOffset, yOffset, zOffset, (int) width, (int) height, (int) depth, gl_format, gl_type, data);
}

void GraphicsAPIImpl::ActiveTexture(uint32_t slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
}

void GraphicsAPIImpl::DeleteTextures(uint32_t count, uint32_t* textureId) {
    glDeleteTextures(count, textureId);
    *textureId = 0;
}

void GraphicsAPIImpl::TexParameteri(TEXTURE_TYPE tex, TEXTURE_PROPERTY texProp, TEXTURE_PARAM texParam) {
    GLenum gl_tex = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_tex_prop = GL::TEXTURE_PROPERTY[static_cast<int>(texProp)];
    GLenum gl_tex_param = GL::TEXTURE_PARAM[static_cast<int>(texParam)];

    glTexParameteri(gl_tex, gl_tex_prop, gl_tex_param);
}

void GraphicsAPIImpl::DrawElements(RENDERER_TYPE rt, uint32_t count, API_TYPE type, const void* indices) {
    GLenum renderer_type = GL::RENDERER_TYPE[static_cast<int>(rt)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glDrawElements(renderer_type, (int) count, gl_type, indices);
}

std::string GraphicsAPIImpl::GetApiVersion() {
    return (const char*)glGetString(GL_VERSION);
}

void GraphicsAPIImpl::EnableBlending() {
    glEnable(GL_BLEND);
}

void GraphicsAPIImpl::BlendFunc(BLEND_PARAM param1, BLEND_PARAM param2) {
    GLenum gl_p1 = GL::BLEND_PARAM[static_cast<int>(param1)];
    GLenum gl_p2 = GL::BLEND_PARAM[static_cast<int>(param2)];

    glBlendFunc(gl_p1, gl_p2);
}

GraphicsAPIImpl& GraphicsAPIImpl::Get()  {
    static GraphicsAPIImpl graphicsApi;
    return graphicsApi;
}

uint32_t GraphicsAPIImpl::GetMaxArrayTexLayers() {
    int layerCount;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &layerCount);
    return (int) layerCount;
}

void GraphicsAPIImpl::CreateFramebuffers(uint32_t count, uint32_t* id) {
    glGenFramebuffers(count, id);
}

void GraphicsAPIImpl::BindFramebuffer(uint32_t id) {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void GraphicsAPIImpl::DeleteFramebuffers(uint32_t count, uint32_t* id) {
    glDeleteFramebuffers(count, id);
}

void GraphicsAPIImpl::CreateRenderbuffers(uint32_t count, uint32_t* id) {
    glGenRenderbuffers(count, id);
}

void GraphicsAPIImpl::BindRenderbuffer(uint32_t id) {
    glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void GraphicsAPIImpl::RenderbufferStorage(INTERNAL_FORMAT depthStencil, uint32_t width, uint32_t height) {
    glRenderbufferStorage(GL_RENDERBUFFER, GL::INTERNAL_FORMAT[static_cast<int>(depthStencil)], (int) width, (int) height);
}

void GraphicsAPIImpl::DeleteRenderbuffers(uint32_t count, uint32_t* id) {
    glDeleteRenderbuffers(count, id);
}

void GraphicsAPIImpl::FramebufferRenderbuffer(BUFFER_TYPE target, INTERNAL_FORMAT internalFormat, BUFFER_TYPE renderBufferTarget, uint32_t renderBufId) {
    glFramebufferRenderbuffer(GL::BUFFER_TYPE[static_cast<int>(target)],
                              GL::INTERNAL_FORMAT[static_cast<int>(internalFormat)],
                              GL::BUFFER_TYPE[static_cast<int>(renderBufferTarget)],
                              renderBufId);
}

void GraphicsAPIImpl::FramebufferTexture(BUFFER_TYPE target, ATTACHMENT attachment, uint32_t texId, uint32_t level) {
    glFramebufferTexture(GL::BUFFER_TYPE[static_cast<int>(target)],
                         GL::ATTACHMENT[static_cast<int>(attachment)],
                         texId,
                         0);
}

void GraphicsAPIImpl::FramebufferTextureLayer(BUFFER_TYPE target, ATTACHMENT attachment, uint32_t texId, uint32_t level,
                                              uint32_t layer) {
    glFramebufferTextureLayer(GL::BUFFER_TYPE[static_cast<int>(target)],
                              GL::ATTACHMENT[static_cast<int>(attachment)],
                              texId,
                              0,
                              layer);
}

void GraphicsAPIImpl::Viewport(int x, int y, int w, int h) {
    glViewport(x, y, w, h);
}

void GraphicsAPIImpl::EnableDepthTest() {
    glEnable(GL_DEPTH_TEST);
}

GLenum GraphicsAPIImpl::GL::ConvertAPITypeToGlType(API_TYPE type) {
    switch (type) {
        case API_TYPE::FLOAT: return GL_FLOAT;
        case API_TYPE::INT:   return GL_INT;
        case API_TYPE::UINT:  return GL_UNSIGNED_INT;
        case API_TYPE::UCHAR: return GL_UNSIGNED_BYTE;
        case API_TYPE::BOOL:  return GL_BOOL;
    }

    return 0;
}
