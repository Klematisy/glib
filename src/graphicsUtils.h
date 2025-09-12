#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <array>
#include <stack>
#include <unordered_map>

#include "environment.h"

#include "OpenGLCore/Renderer.h"
#include "structs.h"

GLIB_NAMESPACE_OPEN

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

GLIB_NAMESPACE_CLOSE