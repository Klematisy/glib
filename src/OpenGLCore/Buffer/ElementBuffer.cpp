#include <iostream>

#include "ElementBuffer.h"

GlCore::ElementBuffer::ElementBuffer(uint32_t bufferType, uint32_t count, const void *data)
    : m_Count(count), m_Capacity(count * sizeof(unsigned int)), m_BufferType(bufferType)
{
    glGenBuffers(1, &m_ID);
    Bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Capacity, data, m_BufferType);
}

void GlCore::ElementBuffer::PutData(uint32_t count, const void *data) {
    if (m_BufferType == GL_STATIC_DRAW) {
        std::cerr << "The buffer is static!" << std::endl;
        return;
    }

    Bind();
    m_Count = count;
    uint32_t size = m_Count * sizeof(unsigned int);

    if (m_Capacity < size) {
        m_Capacity = size;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Capacity, data, m_BufferType);
    } else {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
    }
}

GlCore::ElementBuffer& GlCore::ElementBuffer::operator=(GlCore::ElementBuffer &&other) {
    m_Capacity   = other.m_Capacity;
    m_BufferType = other.m_BufferType;
    m_ID         = other.m_ID;
    m_Count      = other.m_Count;

    other.m_Capacity   = 0;
    other.m_BufferType = 0;
    other.m_ID         = 0;
    other.m_Count      = 0;

    return *this;
}

void GlCore::ElementBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void GlCore::ElementBuffer::UnBind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GlCore::ElementBuffer::~ElementBuffer() {
    UnBind();
    if (m_ID != 0)
        glDeleteBuffers(1, &m_ID);
}

uint32_t GlCore::ElementBuffer::GetCount() const {
    return m_Count;
}
