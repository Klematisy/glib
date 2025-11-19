#pragma once

#include "structs.h"
#include "environment.h"
#include <functional>

#define GLIB_MAX_BATCH_COUNT 10'000

GLIB_NAMESPACE_OPEN

class Batch {
public:
    explicit Batch(uint32_t MAX_BATCH);
    void Clear();
    void ClearVertices();
    void ClearIndices();
    void AddVertices(const Vertex* array, uint32_t size);
    void AddIndices(const uint32_t* array, uint32_t size);

    bool BatchOverflow();

    uint32_t GetVerticesSize();
    const void* GetVerticesData();

    uint32_t GetIndicesSize();
    const void* GetIndicesData();
private:
    uint32_t m_MaxBatchSize = 0;

    std::vector<Vertex>  m_Vertices;
    std::vector<uint32_t> m_Indices;

    uint32_t m_MaxIndex = 0;
};

GLIB_NAMESPACE_CLOSE