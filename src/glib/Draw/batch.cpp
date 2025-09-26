#include <utility>

#include "draw.h"

GLIB_NAMESPACE_OPEN

void Batch::BatchClear() {
    m_Vertices.clear();
    m_Indices.clear();
    m_MaxIndex = 0;
}

void Batch::OverflowCheck() {
    if (MAX_BATCH_SIZE < GetVerticesSize()) {
        m_DrawBuffer();
        BatchClear();
    }
}

void Batch::BatchVertices(const Vertex* array, uint32_t size) {
    OverflowCheck();

    for (uint32_t i = 0; i < size; i++) {
        m_Vertices.push_back(array[i]);
    }
}

void Batch::BatchIndices(const uint32_t* array, uint32_t size) {
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

const void *Batch::GetVerticesData() {
    return m_Vertices.data();
}

uint32_t Batch::GetIndicesSize() {
    return m_Indices.size();
}

const void *Batch::GetIndicesData() {
    return m_Indices.data();
}

uint32_t Batch::GetMaxBatch() {
    return MAX_BATCH_SIZE;
}

void Batch::BindDrawFunc(std::function<void()> func) {
    m_DrawBuffer = std::move(func);
}

GLIB_NAMESPACE_CLOSE