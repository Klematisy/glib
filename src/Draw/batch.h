#pragma once

#include "structs.h"
#include "../environment.h"
#include <functional>

GLIB_NAMESPACE_OPEN

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