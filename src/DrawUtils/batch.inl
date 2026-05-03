#pragma once

template<class t_VertexStruct>
void Batch<t_VertexStruct>::ClearVertices() {
    m_Vertices.clear();
}

template<class t_VertexStruct>
void Batch<t_VertexStruct>::ClearIndices() {
    m_Indices.clear();
    m_MaxIndex = 0;
}

template<class t_VertexStruct>
void Batch<t_VertexStruct>::Clear() {
    m_Vertices.clear();
    m_Indices.clear();
    m_MaxIndex = 0;
}

template<class t_VertexStruct>
bool Batch<t_VertexStruct>::BatchOverflow() {
    return m_MaxBatchSize < GetVerticesCapacity();
}

template<class t_VertexStruct>
void Batch<t_VertexStruct>::AddVertices(const t_VertexStruct* array, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        m_Vertices.push_back(array[i]);
    }
}

template<class t_VertexStruct>
void Batch<t_VertexStruct>::AddIndices(const uint32_t* array, uint32_t size) {
    uint32_t maxIndex = m_MaxIndex;
    for (uint32_t i = 0; i < size; i++) {
        uint32_t el = array[i] + m_MaxIndex;
        m_Indices.push_back(el);
        maxIndex = std::max(el, maxIndex);
    }
    m_MaxIndex = (size > 0) ? ++maxIndex : 0;
}

template<class t_VertexStruct>
uint32_t Batch<t_VertexStruct>::GetVerticesCapacity() const {
    return sizeof(t_VertexStruct) * m_Vertices.size();
}

template<class t_VertexStruct>
const void* Batch<t_VertexStruct>::GetVerticesData() const {
    return m_Vertices.data();
}

template<class t_VertexStruct>
uint32_t Batch<t_VertexStruct>::GetIndicesCount() const {
    return m_Indices.size();
}

template<class t_VertexStruct>
const void* Batch<t_VertexStruct>::GetIndicesData() const {
    return m_Indices.data();
}

template<class t_VertexStruct>
void Batch<t_VertexStruct>::SetMaxBatchSize(uint32_t MAX_BATCH) {
    m_MaxBatchSize = MAX_BATCH;
}