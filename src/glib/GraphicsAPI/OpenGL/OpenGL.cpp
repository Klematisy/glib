#include "OpenGL.h"

GAPI_NAMESPACE_USING

int GraphicsAPIImpl::GraphicsInit() {
    return (int) glewInit();
}

void GraphicsAPIImpl::CreateBuffers(uint32_t count, uint32_t *id) {
    glGenBuffers(count, id);
}

void GraphicsAPIImpl::BufferData(IGraphicsAPI::BUFFER_TYPE bufferType, uint32_t capacity, void *data, IGraphicsAPI::DRAW_BUFFER_TYPE drawType) {
    int bf_index = static_cast<int>(bufferType);
    int dt_index = static_cast<int>(drawType);

    glBufferData(GL::BUFFER_TYPE[bf_index], capacity, data, GL::DRAW_BUFFER_TYPE[dt_index]);
}

void GraphicsAPIImpl::BufferSubData(IGraphicsAPI::BUFFER_TYPE bufferType, ptrdiff_t offset, uint32_t size, void *data) {
    int bf_index = static_cast<int>(bufferType);
    glBufferSubData(GL::BUFFER_TYPE[bf_index], offset, size, data);
}

void GraphicsAPIImpl::BindBuffer(IGraphicsAPI::BUFFER_TYPE bufferType, uint32_t id) {
    int bf_index = static_cast<int>(bufferType);
    glBindBuffer(GL::BUFFER_TYPE[bf_index], id);
}

void GraphicsAPIImpl::DeleteBuffer(uint32_t count, uint32_t* id) {
    glDeleteBuffers(count, id);
    *id = 0;
}

void GraphicsAPIImpl::CreateVertexArray(uint32_t count, uint32_t* id) {
    glGenVertexArrays(count, id);
}

void GraphicsAPIImpl::BindVertexArray(uint32_t id) {
    glBindVertexArray(id);
}

void GraphicsAPIImpl::VertexAttribPointer(uint32_t index, uint32_t size, IGraphicsAPI::API_TYPE type, bool normalized,
                                          uint32_t stride, const void *pointer)
{
    glVertexAttribPointer(index, (int) size, GL::ConvertAPITypeToGlType(type), normalized, (int) stride, pointer);
}

void GraphicsAPIImpl::EnableVertexAttribArray(uint32_t location) {
    glEnableVertexAttribArray(location);
}

void GraphicsAPIImpl::DeleteVertexArrays(uint32_t count, uint32_t* id) {
    glDeleteVertexArrays(count, id);
    *id = 0;
}

uint32_t GraphicsAPIImpl::CreateShader(IGraphicsAPI::SHADER_TYPE type) {
    return glCreateShader(GL::SHADER_TYPE[static_cast<int>(type)]);
}

void GraphicsAPIImpl::ShaderSource(uint32_t id, uint32_t count, const char** src, const int* length) {
    glShaderSource(id, count, src, length);
}

void GraphicsAPIImpl::CompileShader(uint32_t id) {
    glCompileShader(id);
}

void GraphicsAPIImpl::GetShaderiv(uint32_t shader, IGraphicsAPI::SHADER_COMPILE shaderCompile, int* length) {
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

void GraphicsAPIImpl::GetProgramiv(uint32_t program, IGraphicsAPI::SHADER_PROGRAM_COMPILE spc, int* length) {
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

void GraphicsAPIImpl::Uniform1iv(int location, uint32_t count, int* value) {
    glUniform1iv(location, count, value);
}

void GraphicsAPIImpl::UniformMatrix4fv(int location, uint32_t count, IGraphicsAPI::API_BOOLEAN transpose,
                                       const float *matrix) {
    glUniformMatrix4fv(location, count, static_cast<GLboolean>(transpose), matrix);
}

void GraphicsAPIImpl::CreateTexture(uint32_t count, uint32_t* textureId) {
    glGenTextures(count, textureId);
}

void GraphicsAPIImpl::BindTexture(IGraphicsAPI::TEXTURE_TYPE tex, uint32_t textureId) {
    glBindTexture(GL::TEXTURE_TYPE[static_cast<int>(tex)], textureId);
}

void GraphicsAPIImpl::TexImage2D(IGraphicsAPI::TEXTURE_TYPE tex, int level, IGraphicsAPI::INTERNAL_FORMAT int_form,
                                 uint32_t width, uint32_t height, int border, IGraphicsAPI::INTERNAL_FORMAT format,
                                 IGraphicsAPI::API_TYPE type, const void* data)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_int_form = GL::INTERNAL_FORMAT[static_cast<int>(int_form)];
    GLenum gl_format = GL::INTERNAL_FORMAT[static_cast<int>(format)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glTexImage2D(gl_tex_type, level, gl_int_form, (int) width, (int) height, border, gl_format, gl_type, data);
}

void GraphicsAPIImpl::TexSubImage2D(IGraphicsAPI::TEXTURE_TYPE tex, int level, uint32_t xOffset, uint32_t yOffset,
                                    uint32_t width, uint32_t height, IGraphicsAPI::INTERNAL_FORMAT format,
                                    IGraphicsAPI::API_TYPE type, const void* pixels)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_format = GL::INTERNAL_FORMAT[static_cast<int>(format)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glTexSubImage2D(gl_tex_type, level, (int) xOffset, (int) yOffset, (int) width, (int) height, gl_format, gl_type, pixels);
}

void GraphicsAPIImpl::TexImage3D(IGraphicsAPI::TEXTURE_TYPE tex, int level, IGraphicsAPI::INTERNAL_FORMAT int_form,
                                 uint32_t width, uint32_t height, uint32_t layerCount, uint32_t border,
                                 IGraphicsAPI::INTERNAL_FORMAT format, IGraphicsAPI::API_TYPE type, const void *data)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_int_form = GL::INTERNAL_FORMAT[static_cast<int>(int_form)];
    GLenum gl_format = GL::INTERNAL_FORMAT[static_cast<int>(format)];
    GLenum gl_type = GL::ConvertAPITypeToGlType(type);

    glTexImage3D(gl_tex_type, level, gl_int_form, (int) width, (int) height, (int) layerCount, (int) border, gl_format, gl_type, data);
}

void GraphicsAPIImpl::TexSubImage3D(IGraphicsAPI::TEXTURE_TYPE tex, int level, int xOffset, int yOffset, int zOffset,
                                    uint32_t width, uint32_t height, int depth, IGraphicsAPI::INTERNAL_FORMAT format,
                                    IGraphicsAPI::API_TYPE type, const void* data)
{
    GLenum gl_tex_type = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_format = GL::INTERNAL_FORMAT[static_cast<int>(format)];
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

void GraphicsAPIImpl::TexParameteri(IGraphicsAPI::TEXTURE_TYPE tex, IGraphicsAPI::TEXTURE_PROPERTY texProp, IGraphicsAPI::TEXTURE_PARAM texParam) {
    GLenum gl_tex = GL::TEXTURE_TYPE[static_cast<int>(tex)];
    GLenum gl_tex_prop = GL::TEXTURE_PROPERTY[static_cast<int>(texProp)];
    GLenum gl_tex_param = GL::TEXTURE_PARAM[static_cast<int>(texParam)];

    glTexParameteri(gl_tex, gl_tex_prop, gl_tex_param);
}

void GraphicsAPIImpl::DrawElements(IGraphicsAPI::RENDERER_TYPE rt, uint32_t count, IGraphicsAPI::API_TYPE type, const void* indices) {
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

void GraphicsAPIImpl::BlendFunc(IGraphicsAPI::BLEND_PARAM param1, IGraphicsAPI::BLEND_PARAM param2) {
    GLenum gl_p1 = GL::BLEND_PARAM[static_cast<int>(param1)];
    GLenum gl_p2 = GL::BLEND_PARAM[static_cast<int>(param2)];

    glBlendFunc(gl_p1, gl_p2);
}


GLenum GraphicsAPIImpl::GL::ConvertAPITypeToGlType(IGraphicsAPI::API_TYPE type) {
    switch (type) {
        case API_TYPE::FLOAT: return GL_FLOAT;
        case API_TYPE::INT:   return GL_INT;
        case API_TYPE::UINT:  return GL_UNSIGNED_INT;
        case API_TYPE::UCHAR: return GL_UNSIGNED_BYTE;
        case API_TYPE::BOOL:  return GL_BOOL;
    }

    return 0;
}
