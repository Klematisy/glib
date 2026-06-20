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
    void AddVertices(const t_VertexStruct* array, u32 size);
    void AddIndices(const u32* array, u32 size);

    //          ATTENTION           //
    // MAX_BATCH - it is variable   //
    // that stores max batch size   //
    // in BYTES                     //

    bool BatchOverflow();
    void SetMaxBatchSize(u32 MAX_BATCH);

    u32 GetVerticesCapacity() const;
    const void* GetVerticesData() const;

    u32 GetIndicesCount() const;
    const void* GetIndicesData() const;
private:
    std::vector<t_VertexStruct>  m_Vertices;
    std::vector<u32> m_Indices;

    u32 m_MaxIndex = 0;
    u32 m_MaxBatchSize = VLADLIB_MAX_BATCH_COUNT;
};

#include "batch.inl"

VLADLIB_NAMESPACE_CLOSE
