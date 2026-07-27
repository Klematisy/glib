#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "glm/glm.hpp"

#include "common.h"

namespace GAPI {
    enum class API_TYPE { INT, FLOAT, UINT, UCHAR, BOOL };
    enum class API_BOOLEAN : bool { FALSE = false, TRUE = true };

    enum class BUFFER_TYPE { ARRAY, ELEMENT_ARRAY, FRAME, RENDER };
    enum class DRAW_TYPE { STATIC, DYNAMIC };
    enum class RENDERER_TYPE { TRIANGLES, LINES, POINTS };

    enum class SHADER_TYPE : u32 { VERTEX = 1 << 0, FRAGMENT = 1 << 1, COMPUTE = 1 << 2, GEOMETRY = 1 << 3 };
    enum class SHADER_COMPILE { STATUS, INFO_LOG_LENGTH };
    enum class SHADER_PROGRAM_COMPILE { LINK_STATUS, INFO_LOG_LENGTH };

    enum class TEXTURE_TYPE { _2D, _3D, ARRAY };
    enum class FORMAT { RED, GREEN, BLUE, ALPHA, RGB, RGBA };
    enum class INTERNAL_FORMAT { RGB8, RGBA8, DEPTH24_STENCIL8, DEPTH_STENCIL_ATTACHMENT, COLOR_ATTACHMENT0 };
    enum class TEXTURE_PROPERTY { MIN_FILTER, MAG_FILTER, WRAP_S, WRAP_T };
    enum class TEXTURE_PARAM { NEAREST, LINEAR, CLAMP_TO_EDGE };

    enum class CLEAR_BUFFER_BIT : u32 { COLOR = 1 << 0, DEPTH = 1 << 1, STENCIL = 1 << 2 };
    enum class BLEND_PARAM { SRC_ALPHA, ONE_MINUS_SRC_ALPHA };

    inline bool hasType(CLEAR_BUFFER_BIT prop, CLEAR_BUFFER_BIT set) {
        return (static_cast<u32>(prop) & static_cast<u32>(set)) != 0;
    }

    inline bool hasType(SHADER_TYPE prop, SHADER_TYPE set) {
        return (static_cast<u32>(prop) & static_cast<u32>(set)) != 0;
    }

    inline constexpr CLEAR_BUFFER_BIT operator|(CLEAR_BUFFER_BIT a, CLEAR_BUFFER_BIT b) {
        return static_cast<CLEAR_BUFFER_BIT>(
            static_cast<u32>(a) |
            static_cast<u32>(b)
        );
    }

    inline constexpr SHADER_TYPE operator|(SHADER_TYPE a, SHADER_TYPE b) {
        return static_cast<SHADER_TYPE>(
            static_cast<u32>(a) |
            static_cast<u32>(b)
        );
    }

    inline constexpr u32 operator&(SHADER_TYPE a, SHADER_TYPE b) {
        return static_cast<u32>(a) & static_cast<u32>(b);
    }

    using GraphicsObject = u32;

    struct TextureParameters;
    class ImageInfo;
    class Framebuffer;
    class TextureArray;
    class VertexBuffer;
    class ElementBuffer;
    struct LayoutData;
    class VertexArrayLayout;
    class VertexArray;
    class ITexture;
    class Texture2D;
    class TextureArray;
    class Renderbuffer;
    class Framebuffer;
    class Renderer;
    class Shader;
    class ShaderCompiler;
    class ShaderProgram;

    using VertexBufferPTR   = std::shared_ptr<VertexBuffer>;
    using ElementBufferPTR  = std::shared_ptr<ElementBuffer>;
    using VertexArrayPTR    = std::shared_ptr<VertexArray>;
    using FramebufferPTR    = std::shared_ptr<Framebuffer>;
    using RenderbufferPTR   = std::shared_ptr<Renderbuffer>;
    using Texture2DPTR      = std::shared_ptr<Texture2D>;
    using TextureArrayPTR   = std::shared_ptr<TextureArray>;
    using ShaderPTR         = std::shared_ptr<Shader>;
    using ShaderCompilerPTR = std::shared_ptr<ShaderCompiler>;
    using ShaderProgramPTR  = std::shared_ptr<ShaderProgram>;
    using RendererPTR       = std::shared_ptr<Renderer>;

    u0 initGraphicsBackend();
    std::string getShaderLanguageVersion();
    u0 initGraphicsContext(u32 majorV, u32 minorV);
    std::string parseFile(const std::string& filePath);
    std::string getShaderTypeInStr(GAPI::SHADER_TYPE type);
    void blendFunc(GAPI::BLEND_PARAM param1, GAPI::BLEND_PARAM param2);
    std::string getApiVersion();
    i32 getMaxArrayTexLayers();
    i32 enableDepthTest();
    i32 enableBlending();

    u0 AttachFramebufferToRenderbuffer(const Framebuffer& fb, const Renderbuffer& rb, INTERNAL_FORMAT depthStencil);
    u0 AttachTextureToFramebuffer(const Framebuffer& fb, const Texture2D& tex, INTERNAL_FORMAT attachment);
    u0 AttachTextureArrayToFramebuffer(const Framebuffer& fb, const TextureArray& tex, INTERNAL_FORMAT attachment, u32 layer);

    VertexBufferPTR createVertexBuffer(GAPI::DRAW_TYPE drawType, u32 size, const u0* data);
    ElementBufferPTR createElementBuffer(GAPI::DRAW_TYPE drawType, u32 count, const u0* data);
    VertexArrayPTR createVertexArray();
    FramebufferPTR createFramebuffer();
    RenderbufferPTR createRenderbuffer();
    Texture2DPTR createTexture();
    TextureArrayPTR createTexutreArray();
    ShaderPTR createShader(std::string filePath, GAPI::SHADER_TYPE shaderType);
    ShaderCompilerPTR createShaderCompiler();
    ShaderProgramPTR createShaderProgram();
    RendererPTR createRenderer();

    class ContextAPIInfo {
        friend u0 initGraphicsContext(u32 majorV, u32 minorV);
        friend u0 initGraphicsBackend();
    PUBLIC
        inline static ContextAPIInfo& Get() {
            static ContextAPIInfo instance;
            return instance;
        }

#define CONTEXT_ISNT_INITED -1
        inline bool IsContextInited() { return m_MajorV != CONTEXT_ISNT_INITED && m_MinorV != CONTEXT_ISNT_INITED; }
        inline bool IsAPIInited() { return !m_APIName.empty(); }
        inline i32 GetMajorV() { return m_MajorV; }
        inline i32 GetMinorV() { return m_MinorV; }
        inline const std::string& GetAPIName() { return m_APIName; }
    private:
        ContextAPIInfo() = default;

        i32 m_MajorV = CONTEXT_ISNT_INITED;
        i32 m_MinorV = CONTEXT_ISNT_INITED;
        std::string m_APIName;
#undef CONTEXT_ISNT_INITED
    };


    struct TextureParameters {
        bool operator==(const TextureParameters& other) const {
            return wrapT == other.wrapT &&
                   wrapS == other.wrapS &&
                   magFilter == other.magFilter &&
                   minFilter == other.minFilter;
        }

        GAPI::TEXTURE_PARAM magFilter = GAPI::TEXTURE_PARAM::NEAREST;
        GAPI::TEXTURE_PARAM minFilter = GAPI::TEXTURE_PARAM::NEAREST;
        GAPI::TEXTURE_PARAM wrapS = GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE;
        GAPI::TEXTURE_PARAM wrapT = GAPI::TEXTURE_PARAM::CLAMP_TO_EDGE;
    };

    class ImageInfo {
    PUBLIC
        ImageInfo() = default;

        ImageInfo(u32 w, u32 h, u32 bpp, std::shared_ptr<u8> bitmap);

        explicit ImageInfo(const char* fileName);

        TextureParameters texParams;
    READONLY
        u32 r_Width = 0;
        u32 r_Height = 0;
        u32 r_BPP = 0;
        std::shared_ptr<u8> r_Bitmap;
    };

    class VertexBuffer {
    PUBLIC
        VertexBuffer() = default;
        virtual ~VertexBuffer() = default;
        // VertexBuffer(GAPI::DRAW_TYPE bufferType, u32 size, const u0* data) = 0;

        virtual u0 PutData(const u0* data, u32 size) = 0;
        virtual u0 Bind() const = 0;
        virtual u0 UnBind() const = 0;
    PROTECTED
        GAPI::DRAW_TYPE m_DrawType;
        u32 m_Capacity = 0;
        u32 m_ID = 0;
    };

    class ElementBuffer {
    PUBLIC
        ElementBuffer() = default;
        virtual ~ElementBuffer() = default;
        // ElementBuffer(GAPI::DRAW_TYPE drawType, u32 count, const u0* data);

        virtual u0 PutData(const u0* data, u32 count) = 0;
        virtual u0 Bind() const = 0;
        virtual u0 UnBind() const = 0;
        virtual u32 GetCount() const = 0;
    PROTECTED
        u32 m_Count = 0;

        DRAW_TYPE m_DrawType;
        u32 m_ID = 0;
    };

    struct LayoutData {
        u32 size = 0;
        u32 offset = 0;
        GAPI::API_TYPE type = GAPI::API_TYPE::INT;
    };

    class VertexArrayLayout {
    PUBLIC
        template<class T>
        u0 Add(u32 i) {
            m_Layouts.push_back({i, m_Offset});
            m_Offset += i * sizeof(T);
        }

        template<>
        u0 Add<f32>(u32 i) {
            m_Layouts.push_back({i, m_Offset, GAPI::API_TYPE::FLOAT});
            m_Offset += i * sizeof(f32);
        }

        template<>
        u0 Add<unsigned int>(u32 i) {
            m_Layouts.push_back({i, m_Offset, GAPI::API_TYPE::UINT});
            m_Offset += i * sizeof(unsigned int);
        }

        template<>
        u0 Add<unsigned char>(u32 i) {
            m_Layouts.push_back({i, m_Offset, GAPI::API_TYPE::UCHAR});
            m_Offset += i;
        }

        const std::vector<LayoutData>& GetLayouts() const {
            return m_Layouts;
        }

        int GetFullOffset() const {
            return static_cast<int>(m_Offset);
        }
    PROTECTED
        u32 m_Offset = 0;
        std::vector<LayoutData> m_Layouts;
    };

    class VertexArray {
    PUBLIC
        VertexArray() = default;
        virtual ~VertexArray() = default;
        VertexArray& operator=(VertexArray&& other);

        virtual u0 Bind() const = 0;
        virtual u0 UnBind() const = 0;
        virtual u0 AddElementBuffer(const ElementBuffer& vb) = 0;
        virtual u0 AddVertexBuffer(const VertexArrayLayout& layout, const VertexBuffer& vb) = 0;
    PROTECTED
        u32 m_ID = 0;
    };

    class ITexture {
    PUBLIC
        virtual u0 Bind(u32 slot = 0) const = 0;
        virtual u0 UnBind() const = 0;

    READONLY
        u32 r_Width = 0;
        u32 r_Height = 0;

        TextureParameters r_TexParameters;
    PROTECTED
        u32 m_ID = 0;
    };

    class Texture2D : public ITexture {
    PUBLIC
        Texture2D() = default;
        virtual ~Texture2D() = default;

        virtual u0 Upload(const ImageInfo& info) = 0;

        friend u0 AttachTextureToFramebuffer(const Framebuffer& fb,const Texture2D& tex, GAPI::INTERNAL_FORMAT attachment);
        friend u0 AttachTextureArrayToFramebuffer(const Framebuffer& fb, const TextureArray& tex, GAPI::INTERNAL_FORMAT attachment, u32 layer);
    PROTECTED
        u32 m_AllocatedW = 0;
        u32 m_AllocatedH = 0;
    };

    class TextureArray : public ITexture {
    PUBLIC
        TextureArray() = default;
        virtual ~TextureArray() = default;

        virtual u0 AddImage(const ImageInfo& info, u32 xOffset, u32 yOffset, u32 slot) = 0;
        virtual u0 Init(u32 width, u32 height, u32 layersCount, const TextureParameters& tp = {}) = 0;

        friend u0 AttachTextureArrayToFramebuffer(const Framebuffer& fb, const TextureArray& tex, GAPI::INTERNAL_FORMAT attachment, u32 layer);
    READONLY
        u32 r_Layers = 0;
    };

    class Framebuffer {
    PUBLIC
        Framebuffer() = default;
        virtual ~Framebuffer() = default;

        virtual u0 Bind() const = 0;
        virtual u0 UnBind() const = 0;

        friend u0 AttachTextureToFramebuffer(const Framebuffer& fb, const Texture2D& tex, GAPI::INTERNAL_FORMAT attachment);
        friend u0 AttachTextureArrayToFramebuffer(const Framebuffer& fb, const TextureArray& tex, GAPI::INTERNAL_FORMAT attachment, u32 layer);
        friend u0 AttachFramebufferToRenderbuffer(const Framebuffer& fb, const Renderbuffer& rb, GAPI::INTERNAL_FORMAT depthStencil);
    PROTECTED
        u32 m_FBO = 0;
    };

    class Renderbuffer {
    PUBLIC
        Renderbuffer() = default;
        virtual ~Renderbuffer() = default;

        virtual u0 Init(u32 width, u32 height, GAPI::INTERNAL_FORMAT depthStencil) = 0;

        virtual u0 Bind() const = 0;
        virtual u0 UnBind() const = 0;

        friend u0 AttachFramebufferToRenderbuffer(const Framebuffer& fb, const Renderbuffer& rb, GAPI::INTERNAL_FORMAT depthStencil);
    READONLY
        u32 r_Width = 0;
        u32 r_Height = 0;
    PROTECTED
        u32 m_RB = 0;
    };

    class Shader {
        friend class ShaderCompiler;
        friend class ShaderProgram;
    PUBLIC
        Shader() = default;
        virtual ~Shader() = default;

    PROTECTED
        std::string m_FilePath;
        GAPI::SHADER_TYPE m_ShaderType;
        u32 m_Id = 0;
    };

    class ShaderCompiler {
    PUBLIC
        std::vector<std::string> precompiledOptions;

        virtual u0 Compile(Shader* shader) = 0;
    PROTECTED
        virtual i32 CheckShaderErrors(Shader* shader) = 0;
    };

    class ShaderProgram {
    PUBLIC
        ShaderProgram() = default;
        virtual ~ShaderProgram() = default;

        virtual u0 Bind() const = 0;
        virtual u0 UnBind() const = 0;

        virtual u0 AttachShader(const Shader* shader) = 0;
        virtual u0 LinkProgram() = 0;
        virtual u0 ClearShaders() = 0;

        //-----------------------------UNIFORMS-----------------------------//
        virtual u0 SetInt(const std::string& name, int value) const = 0;
        virtual u0 SetFloat(const std::string& name, f32 value) const = 0;
        virtual u0 SetFloat2(const std::string& name, const glm::vec2& val) const = 0;
        virtual u0 SetIntArray(const std::string& name, u32 count, const int* value) const = 0;
        virtual u0 SetMatrixFloat4(const std::string& name, const f32* value_ptr) const = 0;
        //-----------------------------UNIFORMS-----------------------------//

    PROTECTED
        virtual i32 GetUniformLocation(const std::string& name) const = 0;
        virtual i32 CheckLinkingErrors() const = 0;
    PROTECTED
        std::vector<const Shader*> m_AttachedShaders;
        mutable std::unordered_map<std::string, int> m_UniformLocations;
        GraphicsObject m_ShaderProgram = 0 ;
    };

    struct RenderItem {
        std::shared_ptr<VertexArray> vertexArray;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<ElementBuffer> elementBuffer;

        const ShaderProgram* shader = nullptr;
        const ITexture* texture = nullptr;
        GAPI::RENDERER_TYPE renderType = GAPI::RENDERER_TYPE::TRIANGLES;
    };

    struct RenderStats {
        u32 drawCalls = 0;
        u32 triangles = 0;
        u32 textureBinds = 0;
    };

    class Renderer {
    PUBLIC
        Renderer() = default;
        virtual ~Renderer() = default;

        virtual u0 Draw(const RenderItem& e) = 0;
        virtual u0 Clear(CLEAR_BUFFER_BIT bits) = 0;

        virtual RenderStats GetStats() const = 0;
    PROTECTED
        mutable RenderStats m_Stats;
    };
}
