#pragma once

#include "OpenGLCore/Renderer.h"
#include "structs.h"

namespace glib {
    class Shader {
    public:
        Shader() = default;
        Shader(const char *filePath);
        GlCore::ShaderProgram& GetShader();
    private:
        GlCore::ShaderProgram m_CustomShader;
    };

    class Texture {
    public:
        Texture() = default;
        Texture(Texture &&other) noexcept;
        explicit Texture(const char *filePath);
        Texture(int width, int height, uint8_t* bitmap);
        ~Texture();

        Texture& operator=(Texture &&other) noexcept;

        int GetWidth()  const;
        int GetHeight() const;
        int GetBPP()    const;

        uint32_t GetSize()   const;
        uint8_t* GetBitmap() const;
    private:
        uint8_t* m_Bitmap = nullptr;
        int      m_BPP    = 0;
        int      m_Width  = 0;
        int      m_Height = 0;
    };

    struct TexInfo {
        const Texture* tex = nullptr;
        uint32_t xOffset   = 0;
        uint32_t yOffset   = 0;

        static constexpr uint32_t WIDTH_MAX_SIZE  = 3000;
        static constexpr uint32_t HEIGHT_MAX_SIZE = 3000;
        static constexpr uint32_t BPP_MAX_LEN = 4;

        static constexpr uint32_t BUFFER_MAX_SIZE =
                WIDTH_MAX_SIZE * HEIGHT_MAX_SIZE * BPP_MAX_LEN;
    };

    class TextureSlotManager {
    public:
        TextureSlotManager();
        const TexInfo& PushTexture(const Texture *texture);
        const TexInfo& GetTexInfo(const Texture *texture);
        void BindDrawFunc(std::function<void()>);
        void FillTexture(const TexInfo& it);
        void Bind(int slot = 0);
        void Clear();

        const Texture& GetBasicTex() const;
    private:
        const Texture m_BasicTexture = Texture(1, 1, nullptr);

        std::function<void()> m_DrawBuffer;

        std::vector<TexInfo> m_TexsInfo;
        unsigned char* m_CommonBuffer = nullptr;
        uint32_t m_CommonSize = 0;

        GlCore::Texture m_Texture;

        uint32_t m_MaxHeight  = 0;

        uint32_t xPen = 0;
        uint32_t yPen = 0;
    };

    class Batch {
    public:
        Batch() = default;
        void BindDrawFunc(std::function<void()> func);
        void BatchClear();
        void BatchVertices(const Vertex* array, uint32_t size);
        void BatchIndices(const uint32_t* array, uint32_t size);

        void OverflowCheck();

        uint32_t GetVerticesSize();
        const void* GetVerticesData();

        uint32_t GetIndicesSize();
        const void* GetIndicesData();

        static uint32_t GetMaxBatch() ;
    private:
        std::function<void()> m_DrawBuffer;

        static constexpr uint32_t MAX_BATCH_SIZE = 10000;

        std::vector<Vertex>  m_Vertices;
        std::vector<uint32_t> m_Indices;

        uint32_t m_MaxIndex = 0;
    };
}