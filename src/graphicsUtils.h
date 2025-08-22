#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <array>

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

    class TexInfo {
    public:
        TexInfo() = default;
        TexInfo(const Texture* tex, uint32_t xOffset, uint32_t yOffset, uint32_t slot = 0)
            : m_Tex(tex), m_XOffset(xOffset), m_YOffset(yOffset), m_Slot(slot)
        {}

        const Texture* GetTex() const { return m_Tex;     }
        uint32_t GetXOffset()   const { return m_XOffset; }
        uint32_t GetYOffset()   const { return m_YOffset; }
        uint32_t GetSlot()      const { return m_Slot;    }

        static constexpr uint32_t WIDTH_MAX_SIZE  = 3000;
        static constexpr uint32_t HEIGHT_MAX_SIZE = 3000;
        static constexpr uint32_t BPP_MAX_LEN = 4;

        static constexpr uint32_t BUFFER_MAX_SIZE =
                WIDTH_MAX_SIZE * HEIGHT_MAX_SIZE * BPP_MAX_LEN;

    private:
        const Texture* m_Tex = nullptr;
        uint32_t m_XOffset   = 0;
        uint32_t m_YOffset   = 0;
        uint32_t m_Slot      = 0;
    };

    class Slot {
        using InfoArr = std::vector<TexInfo>;
    public:
        Slot()  = default;
        ~Slot() = default;

        const InfoArr& GetInfo() const;
        void FindFreeSpace();

        template<class... Args>
        void EmplaceBack(Args&&... args);
    private:
        InfoArr m_SlotInfo;
    };

    class TextureManager {
    public:
        TextureManager();
        const TexInfo& GetTexInfo(const Texture *texture);
        const TexInfo& PushTexture(const Texture *texture);
        void FillTexture(const TexInfo& it);
        void CreateTexture(uint32_t slot = 0);
        void Bind();
        void Clear();

        const Texture& GetBasicTex() const;

        static constexpr uint32_t LAYERS = 16;

#ifdef __GLIB_DEBUG__
        void PrintTextures();
#endif
    private:
        GlCore::TextureArray m_Textures;
        const Texture m_BasicTexture = Texture(1, 1, nullptr);

        std::array<Slot, LAYERS> m_TexsInfo;
        std::unique_ptr<uint8_t> m_CommonBuffer;

        uint32_t m_FilledSlots = 1;
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