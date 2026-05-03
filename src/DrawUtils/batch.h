#pragma once

#include "environment.h"

#define VLADLIB_MAX_BATCH_COUNT 10'000

VLADLIB_NAMESPACE_OPEN

template<class t_VertexStruct>
class Batch {
public:
    Batch() = default;
    void Clear();
    void ClearVertices();
    void ClearIndices();
    void AddVertices(const t_VertexStruct* array, uint32_t size);
    void AddIndices(const uint32_t* array, uint32_t size);

    //          ATTENTION           //
    // MAX_BATCH - it is variable   //
    // that stores max batch size   //
    // in BYTES                     //

    bool BatchOverflow();
    void SetMaxBatchSize(uint32_t MAX_BATCH);

    uint32_t GetVerticesCapacity() const;
    const void* GetVerticesData() const;

    uint32_t GetIndicesCount() const;
    const void* GetIndicesData() const;
private:
    std::vector<t_VertexStruct>  m_Vertices;
    std::vector<uint32_t> m_Indices;

    uint32_t m_MaxIndex = 0;
    uint32_t m_MaxBatchSize = VLADLIB_MAX_BATCH_COUNT;
};

#include "batch.inl"

VLADLIB_NAMESPACE_CLOSE