#include <iostream>

#include "element_buffer.h"

using namespace RendererCore;
using namespace GAPI;

static GraphicsAPIImpl& gapi = GraphicsAPIImpl::Get();

ElementBuffer::ElementBuffer(GAPI::DRAW_TYPE bufferType, uint32_t count, const void *data)
    : m_Count(count), m_Capacity(count * sizeof(unsigned int)), m_BufferType(bufferType)
{
    gapi.CreateBuffers(1, &m_ID);
    Bind();

    gapi.BufferData(BUFFER_TYPE::ELEMENT_ARRAY, m_Capacity, data, m_BufferType);
}

void ElementBuffer::PutData(uint32_t count, const void *data) {
    if (m_BufferType == DRAW_TYPE::STATIC) {
        std::cerr << "The buffer is static!" << std::endl;
        return;
    }

    Bind();
    m_Count = count;
    uint32_t size = m_Count * sizeof(unsigned int);

    if (m_Capacity < size) {
        m_Capacity = size;
        gapi.BufferData(BUFFER_TYPE::ELEMENT_ARRAY, m_Capacity, data, m_BufferType);
    } else {
        gapi.BufferSubData(BUFFER_TYPE::ELEMENT_ARRAY, 0, size, data);
    }
}

ElementBuffer& ElementBuffer::operator=(ElementBuffer &&other) {
    m_Capacity   = other.m_Capacity;
    m_BufferType = other.m_BufferType;
    m_ID         = other.m_ID;
    m_Count      = other.m_Count;

    other.m_Capacity   = 0;
    other.m_ID         = 0;
    other.m_Count      = 0;

    return *this;
}

void ElementBuffer::Bind() const {
    gapi.BindBuffer(BUFFER_TYPE::ELEMENT_ARRAY, m_ID);
}

void ElementBuffer::UnBind() const {
    gapi.BindBuffer(BUFFER_TYPE::ELEMENT_ARRAY, 0);
}

ElementBuffer::~ElementBuffer() {
    UnBind();
    gapi.DeleteBuffers(1, &m_ID);
}

uint32_t ElementBuffer::GetCount() const {
    return m_Count;
}
