#include <memory>

#include "type_casting.h"

#include "graphics_api_opengl.h"
#include "glfw_window.h"

#ifdef __VLADLIB_OPENGL__

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
    return "#version 410 core\n";
}

i32 GAPI::initGraphicsBackend() {
    return glewInit();
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
std::shared_ptr<Window> GAPI::createWindow() {
    return std::make_shared<WindowGLFW>();
}
#endif

std::shared_ptr<VertexBuffer> GAPI::createVertexBuffer(GAPI::DRAW_TYPE drawType, u32 size, const u0* data) {
    return std::make_shared<VertexBufferOpenGL>(drawType, size, data);
}

std::shared_ptr<ElementBuffer> GAPI::createElementBuffer(GAPI::DRAW_TYPE drawType, u32 count, const u0* data) {
    return std::make_shared<ElementBufferOpenGL>(drawType, count, data);
}

std::shared_ptr<VertexArray> GAPI::createVertexArray() {
    return std::make_shared<VertexArrayOpenGL>();
}

std::shared_ptr<Framebuffer> GAPI::createFramebuffer() {
    return std::make_shared<FramebufferOpenGL>();
}

std::shared_ptr<Renderbuffer> GAPI::createRenderbuffer() {
    return std::make_shared<RenderbufferOpenGL>();
}

std::shared_ptr<Texture2D> GAPI::createTexture() {
    return std::make_shared<Texture2DOpenGL>();
}

std::shared_ptr<TextureArray> GAPI::createTexutreArray() {
    return std::make_shared<TextureArrayOpenGL>();
}

std::shared_ptr<Shader> GAPI::createShader(std::string filePath, GAPI::SHADER_TYPE shaderType) {
    return std::make_shared<ShaderOpenGL>(filePath, shaderType);
}

std::shared_ptr<ShaderCompiler> GAPI::createShaderCompiler() {
    return std::make_shared<ShaderCompilerOpenGL>();
}

std::shared_ptr<ShaderProgram> GAPI::createShaderProgram() {
    return std::make_shared<ShaderProgramOpenGL>();
}

std::shared_ptr<Renderer> GAPI::createRenderer() {
    return std::make_shared<RendererOpenGL>();
}

#endif
