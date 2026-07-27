#pragma once

#include "../graphics_api.h"
#include "GraphicsAPI/OpenGL/preprocessor.h"

using namespace GAPI;

class VertexBufferOpenGL : public VertexBuffer {
public:
    VertexBufferOpenGL() = delete;
    VertexBufferOpenGL(GAPI::DRAW_TYPE drawType, u32 size, const u0* data);
    ~VertexBufferOpenGL();

    VertexBufferOpenGL& operator=(VertexBufferOpenGL&& other);

    u0 PutData(const u0* data, u32 size) override;
    u0 Bind() const override;
    u0 UnBind() const override;
};

class ElementBufferOpenGL : public ElementBuffer {
public:
    ElementBufferOpenGL() = delete;
    ElementBufferOpenGL(GAPI::DRAW_TYPE drawType, u32 count, const u0* data);
    ~ElementBufferOpenGL();

    ElementBufferOpenGL& operator=(ElementBufferOpenGL&& other);

    u0 PutData(const u0* data, u32 count) override;
    u0 Bind() const override;
    u0 UnBind() const override;
    u32 GetCount() const override;
};


class VertexArrayOpenGL : public VertexArray {
public:
    VertexArrayOpenGL();
    ~VertexArrayOpenGL();
    VertexArrayOpenGL& operator=(VertexArrayOpenGL&& other);

    u0 Bind() const override;
    u0 UnBind() const override;
    u0 AddElementBuffer(const ElementBuffer& vb) override;
    u0 AddVertexBuffer(const VertexArrayLayout& layout, const VertexBuffer& vb) override;
};

class FramebufferOpenGL : public Framebuffer {
public:
    FramebufferOpenGL();
    ~FramebufferOpenGL();

    u0 Bind() const override;
    u0 UnBind() const override;
};


class RenderbufferOpenGL : public Renderbuffer {
public:
    RenderbufferOpenGL();
    ~RenderbufferOpenGL();

    u0 Init(u32 width, u32 height, GAPI::INTERNAL_FORMAT depthStencil) override;

    u0 Bind() const override;
    u0 UnBind() const override;
};

class Texture2DOpenGL : public Texture2D {
public:
    Texture2DOpenGL();
    ~Texture2DOpenGL();

    Texture2DOpenGL& operator=(Texture2DOpenGL&& other);
    Texture2DOpenGL(Texture2DOpenGL&& other);

    u0 Upload(const ImageInfo& info) override;

    u0 Bind(u32 slot = 0) const override;
    u0 UnBind() const override;
};

class TextureArrayOpenGL : public TextureArray {
public:
    TextureArrayOpenGL();
    TextureArrayOpenGL(TextureArrayOpenGL&& other) noexcept;
    TextureArrayOpenGL& operator=(TextureArrayOpenGL&& other);
    ~TextureArrayOpenGL();

    u0 Init(u32 width, u32 height, u32 layersCount, const TextureParameters& tp = {}) override;
    u0 AddImage(const ImageInfo& info, u32 xOffset, u32 yOffset, u32 slot) override;

    u0 Bind(u32 slot = 0) const override;
    u0 UnBind() const override;
};


class ShaderOpenGL : public Shader {
    friend class ShaderCompilerOpenGL;
    friend class ShaderProgramOpenGL;
public:
    ShaderOpenGL(ShaderOpenGL&& other) noexcept;
    ShaderOpenGL& operator=(ShaderOpenGL&& other) noexcept;

    ShaderOpenGL(std::string filePath, GAPI::SHADER_TYPE shaderType);
    ~ShaderOpenGL();
};

class ShaderCompilerOpenGL : public ShaderCompiler {
public:
    u0 Compile(Shader* shader) override;
private:
    void convertErrorLogToReadable(const std::vector<PreProcessor::FileField>& fields, i32 i);
    i32 CheckShaderErrors(Shader* shader) override;
    mutable std::string m_ErrorLog;
};

class ShaderProgramOpenGL : public ShaderProgram {
public:
    ShaderProgramOpenGL();
    ~ShaderProgramOpenGL();

    ShaderProgramOpenGL& operator=(const ShaderProgramOpenGL& other);
    ShaderProgramOpenGL& operator=(ShaderProgramOpenGL&& other);

    u0 Bind() const override;
    u0 UnBind() const override;

    u0 AttachShader(const Shader* shader) override;
    u0 LinkProgram() override;
    u0 ClearShaders() override;

    //-----------------------------UNIFORMS-----------------------------//
    u0 SetInt(const std::string& name, int value) const override;
    u0 SetFloat(const std::string& name, float value) const override;
    u0 SetFloat2(const std::string& name, const glm::vec2& val) const override;
    u0 SetIntArray(const std::string& name, u32 count, const int* value) const override;
    u0 SetMatrixFloat4(const std::string& name, const float* value_ptr) const override;
    //-----------------------------UNIFORMS-----------------------------//

private:
    i32 GetUniformLocation(const std::string& name) const override;
    i32 CheckLinkingErrors() const override;
};


class RendererOpenGL : public Renderer {
public:
    u0 Draw(const RenderItem& item) override;
    u0 Clear(CLEAR_BUFFER_BIT bits) override;
    RenderStats GetStats() const override;
};
