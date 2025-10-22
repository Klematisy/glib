#include <iostream>

#include "vertex_buffer.h"

GlCore::VertexBuffer::VertexBuffer(uint32_t bufferType, uint32_t size, const void *data)
    : m_Capacity(size), m_BufferType(bufferType)
{
    glGenBuffers(1, &m_ID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, m_Capacity, data, m_BufferType);
}

void GlCore::VertexBuffer::PutData(uint32_t size, const void* data) {
    if (m_BufferType == GL_STATIC_DRAW) {
        std::cerr << "The buffer is static!" << std::endl;
        return;
    }

    Bind();

    if (m_Capacity < size) {
        m_Capacity = size;
        glBufferData(GL_ARRAY_BUFFER, m_Capacity, data, m_BufferType);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}

GlCore::VertexBuffer& GlCore::VertexBuffer::operator=(GlCore::VertexBuffer &&other) {
    m_Capacity   = other.m_Capacity;
    m_ID         = other.m_ID;
    m_BufferType = other.m_BufferType;

    other.m_Capacity   = 0;
    other.m_ID         = 0;
    other.m_BufferType = 0;

    return *this;
}

void GlCore::VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void GlCore::VertexBuffer::UnBind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GlCore::VertexBuffer::~VertexBuffer() {
    UnBind();
    if (m_ID != 0)
        glDeleteBuffers(1, &m_ID);
}
