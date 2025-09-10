#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <array>
#include <unordered_map>

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
        Texture(int width, int height);
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

        void SetTex(const Texture* tex) { m_Tex = tex;   }
        void SetXOffset(uint32_t x)     { m_XOffset = x; }
        void SetYOffset(uint32_t y)     { m_YOffset = y; }
        void SetSlot(uint32_t slot)     { m_Slot = slot; }

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
        template<class T>
        using vec = std::vector<T>;

        template<class KEY, class VALUE>
        using map = std::unordered_map<KEY, VALUE>;
        struct Row;
    public:
        Slot();
        ~Slot() = default;

        const map<uint32_t, Row>& GetInfo() const;
        const uint8_t* GetData() const;
        bool PushBack(const TexInfo& info);

    private:
        void Sort(uint32_t key);
        void Cut(uint32_t key);

        void FillRow(uint32_t key);
        void FillImage(const TexInfo& info);

        bool FindFreeSpace(const TexInfo& tex);
    private:
        struct Row {
            vec<TexInfo> images;
            uint32_t maxHeight = 0;
        };

        map<uint32_t, Row> m_Rows;

        std::unique_ptr<uint8_t> m_CommonBuffer;
        vec<Rectangle> m_FreeRects;

        uint32_t m_MaxHeight = 0;
        uint32_t m_XPen = 0;
        uint32_t m_YPen = 0;
    };

    class TextureManager {
    public:
        TextureManager();
        const TexInfo& GetTexInfo(const Texture *texture);
        void Bind();

        const Texture& GetBasicTex() const;

        static constexpr uint32_t LAYERS = 16;
        static constexpr uint32_t FIRST_SLOT = 1;

#ifdef __GLIB_DEBUG__
        void PrintTextures(int i);
#endif
    private:
        const TexInfo& PushTexture(const Texture *texture);

        GlCore::TextureArray m_Textures;
        const Texture m_BasicTexture = Texture(1, 1, nullptr);

        std::array<Slot, LAYERS + FIRST_SLOT> m_TexsInfo;

        uint32_t m_FilledSlots = FIRST_SLOT;

        TexInfo m_LastCreatedEl {0, 0, 0, 0};
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