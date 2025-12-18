#pragma once

#include <utility>
#include <vector>
#include <memory>

#define GLEW_STATIC
#include "GL/glew.h"
#include "graphics_api.h"

GAPI_NAMESPACE_OPEN

class GraphicsAPIImpl : public IGraphicsAPI {
private:
    struct GL {
        static constexpr int ATTACHMENT[] = { GL_DEPTH_STENCIL_ATTACHMENT, GL_COLOR_ATTACHMENT0 };

        static constexpr int BUFFER_TYPE[] = { GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, GL_FRAMEBUFFER, GL_RENDERBUFFER };
        static constexpr int DRAW_TYPE[] = { GL_STATIC_DRAW, GL_DYNAMIC_DRAW };

        static constexpr int RENDERER_TYPE[] = { GL_TRIANGLES, GL_LINES, GL_POINTS };
        static constexpr int CLEAR_BUFFER_BIT[] = { GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_STENCIL_BUFFER_BIT };

        static constexpr int SHADER_TYPE[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER, GL_GEOMETRY_SHADER };
        static constexpr int SHADER_COMPILE[] = { GL_COMPILE_STATUS, GL_INFO_LOG_LENGTH };
        static constexpr int SHADER_PROGRAM_COMPILE[] = { GL_LINK_STATUS, GL_INFO_LOG_LENGTH };

        static constexpr int TEXTURE_TYPE[] = { GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_2D_ARRAY };
        static constexpr int INTERNAL_FORMAT[] = { GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_RGB8, GL_RGBA, GL_RGBA8, GL_DEPTH24_STENCIL8 };
        static constexpr int TEXTURE_PROPERTY[] = { GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T };
        static constexpr int TEXTURE_PARAM[] = { GL_NEAREST, GL_LINEAR, GL_CLAMP_TO_EDGE };

        static constexpr int BLEND_PARAM[] = { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };

        static GLenum ConvertAPITypeToGlType(API_TYPE type);
    };
public:
    static GraphicsAPIImpl& Get();
    int GraphicsInit() override;

    void CreateBuffers(uint32_t count, uint32_t* id) override;
    void BufferData(BUFFER_TYPE bufferType, uint32_t capacity, const void* data, DRAW_TYPE drawType) override;
    void BufferSubData(BUFFER_TYPE bufferType, ptrdiff_t offset, uint32_t size, const void* data) override;
    void BindBuffer(BUFFER_TYPE bufferType, uint32_t ID) override;
    void DeleteBuffers(uint32_t count, uint32_t* id) override;

    void CreateFramebuffers(uint32_t count, uint32_t* id) override;
    void BindFramebuffer(uint32_t id) override;
    void DeleteFramebuffers(uint32_t count, uint32_t* id) override;

    void FramebufferRenderbuffer(BUFFER_TYPE target, INTERNAL_FORMAT internalFormat, BUFFER_TYPE renderBufferTarget, uint32_t renderBufId) override;
    void FramebufferTexture(BUFFER_TYPE target, ATTACHMENT attachment, uint32_t texId, uint32_t level) override;
    void FramebufferTextureLayer(BUFFER_TYPE target, ATTACHMENT attachment, uint32_t texId, uint32_t level, uint32_t layer) override;

    void CreateRenderbuffers(uint32_t count, uint32_t* id) override;
    void BindRenderbuffer(uint32_t id) override;
    void RenderbufferStorage(INTERNAL_FORMAT depthStencil, uint32_t width, uint32_t height) override;
    void DeleteRenderbuffers(uint32_t count, uint32_t* id) override;

    void CreateVertexArrays(uint32_t count, uint32_t* id) override;
    void BindVertexArray(uint32_t id) override;
    void VertexAttribPointer(uint32_t index, uint32_t size, API_TYPE type, API_BOOLEAN normalized, uint32_t stride, const void* pointer) override;
    void EnableVertexAttribArray(uint32_t location) override;
    void DeleteVertexArrays(uint32_t count, uint32_t* id) override;

    uint32_t CreateShader(SHADER_TYPE type) override;
    void ShaderSource(uint32_t id, uint32_t count, const char** src, const int* length) override;
    void CompileShader(uint32_t id) override;
    void GetShaderiv(uint32_t shader, SHADER_COMPILE shaderCompile, int* length) override;
    void GetShaderInfoLog(uint32_t shader, int bufferSize, int* length, char* msg) override;
    void DeleteShader(uint32_t* id) override;

    uint32_t CreateProgram() override;
    void AttachShader(uint32_t program, uint32_t shader) override;
    void LinkProgram(uint32_t program) override;
    void GetProgramiv(uint32_t program, SHADER_PROGRAM_COMPILE spc, int* length) override;
    void GetProgramInfoLog(uint32_t program, int bufferSize, int* length, char* msg) override;
    void UseProgram(uint32_t program) override;
    void DeleteProgram(uint32_t* program) override;

    int GetUniformLocation(uint32_t program, const char* uniformName) override;
    void Uniform1i(int location, int value) override;
    void Uniform1f(int program, float value) override;
    void Uniform1iv(int location, uint32_t count, const int* value) override;
    void UniformMatrix4fv(int location, uint32_t count, API_BOOLEAN transpose, const float* matrix) override;

    void CreateTextures(uint32_t count, uint32_t* textureId) override;
    void BindTexture(TEXTURE_TYPE tex, uint32_t textureId) override;
    void TexImage2D(TEXTURE_TYPE tex, int level, INTERNAL_FORMAT int_form, uint32_t width, uint32_t height, int border, INTERNAL_FORMAT format, API_TYPE type, const void* data) override;
    void TexSubImage2D(TEXTURE_TYPE tex, int level, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, INTERNAL_FORMAT format, API_TYPE type, const void* pixels) override;
    void TexImage3D(TEXTURE_TYPE tex, int level, INTERNAL_FORMAT int_form, uint32_t width, uint32_t height, uint32_t layerCount, uint32_t border, INTERNAL_FORMAT format, API_TYPE type, const void* data) override;
    void TexSubImage3D(TEXTURE_TYPE tex, int level, int xOffset, int yOffset, int zOffset, uint32_t width, uint32_t height, int depth, INTERNAL_FORMAT format, API_TYPE type, const void* pixels) override;
    void ActiveTexture(uint32_t slot) override;
    void DeleteTextures(uint32_t count, uint32_t* textureId) override;
    void TexParameteri(TEXTURE_TYPE tex, TEXTURE_PROPERTY texProp, TEXTURE_PARAM texParam) override;

    void DrawElements(RENDERER_TYPE rt, uint32_t count, API_TYPE type, const void* indices) override;
    std::string GetApiVersion() override;
    void EnableBlending() override;
    void EnableDepthTest() override;
    uint32_t GetMaxArrayTexLayers() override;
    void Viewport(int x, int y, int w, int h) override;

    template<class... BufferBits>
    void Clear(BufferBits... bufferBits);

    void BlendFunc(BLEND_PARAM param1, BLEND_PARAM param2) override;
private:
    GraphicsAPIImpl() = default;
};


template<class... BufferBits>
void GraphicsAPIImpl::Clear(BufferBits... bufferBits) {
    std::vector<CLEAR_BUFFER_BIT> bits {std::forward<BufferBits>(bufferBits)...};
    GLbitfield result = 0;

    if (bits.size() > 0) {
        result = GL::CLEAR_BUFFER_BIT[static_cast<int>(bits[0])];
        for (uint32_t i = 1; i < bits.size(); i++) {
            result |= GL::CLEAR_BUFFER_BIT[static_cast<int>(bits[i])];
        }
    }

    glClear(result);
}

GAPI_NAMESPACE_CLOSE