#pragma once

#include <cinttypes>
#include <cstddef>
#include <string>

#define GAPI_NAMESPACE_OPEN  namespace GAPI {
#define GAPI_NAMESPACE_CLOSE }
#define GAPI_NAMESPACE_USING using namespace GAPI;

GAPI_NAMESPACE_OPEN

//              IGraphicsAPI              //
/*    IGraphicsAPI - the Interface that lets
 *  you to use your own graphics API.
 *  In standard - that interface uses for
 *  OpenGL.
//                                        //*/

class IGraphicsAPI {
public:
    enum class API_TYPE { INT, FLOAT, UINT, UCHAR, BOOL };
    enum class API_BOOLEAN { FALSE = 0, TRUE = 1 };

    enum class BUFFER_TYPE { ARRAY, ELEMENT_ARRAY };
    enum class DRAW_BUFFER_TYPE { STATIC, DYNAMIC };
    enum class RENDERER_TYPE { TRIANGLES, LINES, POINTS };

    enum class SHADER_TYPE { VERTEX, FRAGMENT, COMPUTE, GEOMETRY };
    enum class SHADER_COMPILE { STATUS, INFO_LOG_LENGTH };
    enum class SHADER_PROGRAM_COMPILE { LINK_STATUS, INFO_LOG_LENGTH };

    enum class TEXTURE_TYPE { _2D, _3D, ARRAY };
    enum class INTERNAL_FORMAT { RED, GREEN, BLUE, ALPHA, RGB, RGB8, RGBA, RGBA8 };
    enum class TEXTURE_PROPERTY { MIN_FILTER, MAG_FILTER, WRAP_S, WRAP_T };
    enum class TEXTURE_PARAM { NEAREST, LINEAR, CLAMP_TO_EDGE };

    enum class CLEAR_BUFFER_BIT { COLOR, DEPTH, STENCIL };
    enum class BLEND_PARAM { SRC_ALPHA, ONE_MINUS_SRC_ALPHA };

    virtual int GraphicsInit() = 0;

    virtual void CreateBuffers(uint32_t count, uint32_t* id) = 0;
    virtual void BufferData(BUFFER_TYPE bufferType, uint32_t capacity, void* data, DRAW_BUFFER_TYPE drawType) = 0;
    virtual void BufferSubData(BUFFER_TYPE bufferType, ptrdiff_t offset, uint32_t size, void* data) = 0;
    virtual void BindBuffer(BUFFER_TYPE bufferType, uint32_t ID) = 0;
    virtual void DeleteBuffer(uint32_t count, uint32_t* id) = 0;

    virtual void CreateVertexArray(uint32_t count, uint32_t* id) = 0;
    virtual void BindVertexArray(uint32_t id) = 0;
    virtual void VertexAttribPointer(uint32_t index, uint32_t size, API_TYPE type, bool normalized, uint32_t stride, const void* pointer) = 0;
    virtual void EnableVertexAttribArray(uint32_t location) = 0;
    virtual void DeleteVertexArrays(uint32_t count, uint32_t* id) = 0;

    virtual uint32_t CreateShader(SHADER_TYPE type) = 0;
    virtual void ShaderSource(uint32_t id, uint32_t count, const char** src, const int* length) = 0;
    virtual void CompileShader(uint32_t id) = 0;
    virtual void GetShaderiv(uint32_t shader, SHADER_COMPILE shaderCompile, int* length) = 0;
    virtual void GetShaderInfoLog(uint32_t shader, int bufferSize, int* length, char* msg) = 0;
    virtual void DeleteShader(uint32_t* id) = 0;

    virtual uint32_t CreateProgram() = 0;
    virtual void AttachShader(uint32_t program, uint32_t shader) = 0;
    virtual void LinkProgram(uint32_t program) = 0;
    virtual void GetProgramiv(uint32_t program, SHADER_PROGRAM_COMPILE spc, int* length) = 0;
    virtual void GetProgramInfoLog(uint32_t program, int bufferSize, int* length, char* msg) = 0;
    virtual void UseProgram(uint32_t program) = 0;
    virtual void DeleteProgram(uint32_t* program) = 0;

    virtual int GetUniformLocation(uint32_t program, const char* uniformName) = 0;
    virtual void Uniform1i(int location, int value) = 0;
    virtual void Uniform1f(int program, float value) = 0;
    virtual void Uniform1iv(int location, uint32_t count, int* value) = 0;
    virtual void UniformMatrix4fv(int location, uint32_t count, API_BOOLEAN transpose, const float* matrix) = 0;

    virtual void CreateTexture(uint32_t count, uint32_t* textureId) = 0;
    virtual void BindTexture(TEXTURE_TYPE tex, uint32_t textureId) = 0;
    virtual void TexImage2D(TEXTURE_TYPE tex, int level, INTERNAL_FORMAT int_form, uint32_t width, uint32_t height, int border, INTERNAL_FORMAT format, API_TYPE type, const void* data) = 0;
    virtual void TexSubImage2D(TEXTURE_TYPE tex, int level, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height, INTERNAL_FORMAT format, API_TYPE type, const void* pixels) = 0;
    virtual void TexImage3D(TEXTURE_TYPE tex, int level, INTERNAL_FORMAT int_form, uint32_t width, uint32_t height, uint32_t layerCount, uint32_t border, INTERNAL_FORMAT format, API_TYPE type, const void* data) = 0;
    virtual void TexSubImage3D(TEXTURE_TYPE tex, int level, int xOffset, int yOffset, int zOffset, uint32_t width, uint32_t height, int depth, INTERNAL_FORMAT format, API_TYPE type, const void* pixels) = 0;
    virtual void ActiveTexture(uint32_t slot) = 0;
    virtual void DeleteTextures(uint32_t count, uint32_t* textureId) = 0;
    virtual void TexParameteri(TEXTURE_TYPE tex, TEXTURE_PROPERTY texProp, TEXTURE_PARAM texParam) = 0;

    virtual void DrawElements(RENDERER_TYPE rt, uint32_t count, API_TYPE type, const void* indices) = 0;
    virtual std::string GetApiVersion() = 0;
    virtual void EnableBlending() = 0;

    /* !!!THIS ALSO NEED TO IMPLEMENT!!!
     * template<class... BufferBits>
     * void Clear(BufferBits&... bufferBits);
     //!!!THIS ALSO NEED TO IMPLEMENT!!!*/

    virtual void BlendFunc(BLEND_PARAM param1, BLEND_PARAM param2) = 0;
};


GAPI_NAMESPACE_CLOSE