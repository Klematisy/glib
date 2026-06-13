#pragma once

// #ifdef __VLADLIB_DEBUG__
#define GLEW_STATIC
#include "GL/glew.h"
// #endif

#include "graphics_api.h"

GAPI_NAMESPACE_OPEN

class GAPI_OpenGL : public IGraphicsAPI {
public:
    static GAPI_OpenGL& Get();
    int Init() override;

    void CreateBuffer(uint32_t* id) override;
    void BufferData(BUFFER_TYPE bufferType, uint32_t capacity, const void* data, DRAW_TYPE drawType) override;
    void BufferSubData(BUFFER_TYPE bufferType, ptrdiff_t offset, uint32_t size, const void* data) override;
    void BindBuffer(BUFFER_TYPE bufferType, uint32_t ID) override;
    void DeleteBuffer(uint32_t* id) override;

    void CreateFramebuffer(uint32_t* id) override;
    void BindFramebuffer(uint32_t id) override;
    void DeleteFramebuffer(uint32_t* id) override;

    void FramebufferRenderbuffer(BUFFER_TYPE target, INTERNAL_FORMAT internalFormat, BUFFER_TYPE renderBufferTarget, uint32_t renderBufId) override;
    void FramebufferTexture(BUFFER_TYPE target, INTERNAL_FORMAT attachment, uint32_t texId, uint32_t level) override;
    void FramebufferTextureLayer(BUFFER_TYPE target, INTERNAL_FORMAT attachment, uint32_t texId, uint32_t level, uint32_t layer) override;

    void CreateRenderbuffer(uint32_t* id) override;
    void BindRenderbuffer(uint32_t id) override;
    void RenderbufferStorage(INTERNAL_FORMAT depthStencil, uint32_t width, uint32_t height) override;
    void DeleteRenderbuffer(uint32_t* id) override;

    void CreateVertexArray(uint32_t* id) override;
    void BindVertexArray(uint32_t id) override;
    void VertexAttribPointer(uint32_t index, uint32_t size, API_TYPE type, API_BOOLEAN normalized, uint32_t stride, const void* pointer) override;
    void EnableVertexAttribArray(uint32_t location) override;
    void DeleteVertexArray(uint32_t* id) override;

    void CreateShader(uint32_t* id, SHADER_TYPE type) override;
    void ShaderSource(uint32_t id, uint32_t count, const char** src, const int* length) override;
    void CompileShader(uint32_t id) override;
    void GetShaderiv(uint32_t shader, SHADER_COMPILE shaderCompile, int* length) override;
    void GetShaderInfoLog(uint32_t shader, int bufferSize, int* length, char* msg) override;
    void DeleteShader(uint32_t* id) override;

    void CreateProgram(uint32_t* id) override;
    void AttachShader(uint32_t program, uint32_t shader) override;
    void LinkProgram(uint32_t program) override;
    void GetProgramiv(uint32_t program, SHADER_PROGRAM_COMPILE spc, int* length) override;
    void GetProgramInfoLog(uint32_t program, int bufferSize, int* length, char* msg) override;
    void UseProgram(uint32_t program) override;
    void DeleteProgram(uint32_t* program) override;

    int GetUniformLocation(uint32_t program, const char* uniformName) override;
    void Uniform1i(int location, int value) override;
    void Uniform1f(int program, float value) override;
    void Uniform2f(int location, float value, float value2) override;
    void Uniform1iv(int location, uint32_t count, const int* value) override;
    void UniformMatrix4fv(int location, uint32_t count, API_BOOLEAN transpose, const float* matrix) override;

    void CreateTextures(uint32_t count, uint32_t* textureId) override;
    void BindTexture(TEXTURE_TYPE tex, uint32_t textureId) override;
    void TexImage2D(TEXTURE_TYPE tex, int level, INTERNAL_FORMAT int_form, uint32_t width, uint32_t height, int border, FORMAT format, API_TYPE type, const void* data) override;
    void TexSubImage2D(TEXTURE_TYPE tex, int level, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, FORMAT format, API_TYPE type, const void* pixels) override;
    void TexImage3D(TEXTURE_TYPE tex, int level, INTERNAL_FORMAT int_form, uint32_t width, uint32_t height, uint32_t layerCount, uint32_t border, FORMAT format, API_TYPE type, const void* data) override;
    void TexSubImage3D(TEXTURE_TYPE tex, int level, int xOffset, int yOffset, int zOffset, uint32_t width, uint32_t height, int depth, FORMAT format, API_TYPE type, const void* pixels) override;
    void ActiveTexture(uint32_t slot) override;
    void DeleteTextures(uint32_t count, uint32_t* textureId) override;
    void TexParameteri(TEXTURE_TYPE tex, TEXTURE_PROPERTY texProp, TEXTURE_PARAM texParam) override;

    void DrawElements(RENDERER_TYPE rt, uint32_t count, API_TYPE type, const void* indices) override;
    std::string GetApiVersion() override;
    void EnableBlending() override;
    void EnableDepthTest() override;
    uint32_t GetMaxArrayTexLayers() override;
    void Viewport(int x, int y, int w, int h) override;

    void Clear(CLEAR_BUFFER_BIT bits) override;
    void BlendFunc(BLEND_PARAM param1, BLEND_PARAM param2) override;

    std::string GetShaderLanguageVersion(IGraphicsAPIContext* context) override;
private:
    GAPI_OpenGL() = default;
};

GAPI_NAMESPACE_CLOSE
