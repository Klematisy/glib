#include <memory>

#include "GraphicsAPI/graphics_api.h"
#include "type_casting.h"

#include "graphics_api_opengl.h"
#include "glfw_window.h"

#define createPTR std::make_shared

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

std::string GAPI::getShaderLanguageVersion() {
    std::string result = "#version ";
    auto& context = ContextAPIInfo::Get();
    result += std::to_string(context.GetMajorV());
    result += std::to_string(context.GetMinorV());
    result += "0 core\n"; // TODO: обдумать

    return result;
}

u0 GAPI::initGraphicsBackend() {
    auto& context = ContextAPIInfo::Get();
    context.m_APIName = "GLEW OpenGL";
}

std::string GAPI::getApiVersion() {
    return (const char*)glGetString(GL_VERSION);
}

i32 GAPI::enableDepthTest() {
    glEnable(GL_DEPTH_TEST);
    return 0;
}

void GAPI::blendFunc(GAPI::BLEND_PARAM param1, GAPI::BLEND_PARAM param2) {
    glBlendFunc(GL::BLEND_PARAM[TO_INT(param1)], GL::BLEND_PARAM[TO_INT(param2)]);
}

i32 GAPI::getMaxArrayTexLayers() {
    i32 layerCount;
    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &layerCount);
    return (i32) layerCount;
}

i32 GAPI::enableBlending() {
    glEnable(GL_BLEND);
    return 0;
}

#ifdef __VLADLIB_GLFW__
WindowPTR GAPI::createWindow(u32 w, u32 h, const char* name) {
    return createPTR<WindowGLFW>(w, h, name);
}
#endif

VertexBufferPTR GAPI::createVertexBuffer(GAPI::DRAW_TYPE drawType, u32 size, const u0* data) {
    return createPTR<VertexBufferOpenGL>(drawType, size, data);
}

ElementBufferPTR GAPI::createElementBuffer(GAPI::DRAW_TYPE drawType, u32 count, const u0* data) {
    return createPTR<ElementBufferOpenGL>(drawType, count, data);
}

VertexArrayPTR GAPI::createVertexArray() {
    return createPTR<VertexArrayOpenGL>();
}

FramebufferPTR GAPI::createFramebuffer() {
    return createPTR<FramebufferOpenGL>();
}

RenderbufferPTR GAPI::createRenderbuffer() {
    return createPTR<RenderbufferOpenGL>();
}

Texture2DPTR GAPI::createTexture() {
    return createPTR<Texture2DOpenGL>();
}

TextureArrayPTR GAPI::createTexutreArray() {
    return createPTR<TextureArrayOpenGL>();
}

ShaderPTR GAPI::createShader(std::string filePath, GAPI::SHADER_TYPE shaderType) {
    return createPTR<ShaderOpenGL>(filePath, shaderType);
}

ShaderCompilerPTR GAPI::createShaderCompiler() {
    return createPTR<ShaderCompilerOpenGL>();
}

ShaderProgramPTR GAPI::createShaderProgram() {
    return createPTR<ShaderProgramOpenGL>();
}

RendererPTR GAPI::createRenderer() {
    return createPTR<RendererOpenGL>();
}
