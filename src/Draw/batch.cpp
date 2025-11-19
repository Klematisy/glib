#include <utility>

#include "batch.h"

GLIB_NAMESPACE_OPEN

Batch::Batch(uint32_t MAX_BATCH)
    : m_MaxBatchSize(MAX_BATCH)
{}

    void AddVertices(const Vertex* array, uint32_t size);
    void AddIndices(const uint32_t* array, uint32_t size);

void Batch::ClearVertices() {
    m_Vertices.clear();
}
void Batch::ClearIndices() {
    m_Indices.clear();
    m_MaxIndex = 0;
}

void Batch::Clear() {
    m_Vertices.clear();
    m_Indices.clear();
    m_MaxIndex = 0;
}

bool Batch::BatchOverflow() {
    return m_MaxBatchSize < GetVerticesSize();
}

void Batch::AddVertices(const Vertex* array, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        m_Vertices.push_back(array[i]);
    }
}

void Batch::AddIndices(const uint32_t* array, uint32_t size) {
    uint32_t maxIndex = m_MaxIndex;
    for (uint32_t i = 0; i < size; i++) {
        uint32_t el = array[i] + m_MaxIndex;
        m_Indices.push_back(el);
        maxIndex = std::max(el, maxIndex);
    }
    m_MaxIndex = ++maxIndex;
}

uint32_t Batch::GetVerticesSize() {
    return m_Vertices.size();
}

const void* Batch::GetVerticesData() {
    return m_Vertices.data();
}

uint32_t Batch::GetIndicesSize() {
    return m_Indices.size();
}

const void* Batch::GetIndicesData() {
    return m_Indices.data();
}

GLIB_NAMESPACE_CLOSE