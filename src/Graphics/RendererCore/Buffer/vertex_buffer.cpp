#include <iostream>

#include "vertex_buffer.h"

using namespace RendererCore;
using namespace GAPI;
static auto& gapi = GraphicsAPIImpl::Get();

VertexBuffer::VertexBuffer(GAPI::DRAW_TYPE bufferType, uint32_t size, const void *data)
    : m_BufferType(bufferType), m_Capacity(size)
{
    gapi.CreateBuffers(1, &m_ID);
    Bind();
    gapi.BufferData(BUFFER_TYPE::ARRAY, m_Capacity, data, m_BufferType);
}

void VertexBuffer::PutData(uint32_t size, const void* data) {
    if (m_BufferType == DRAW_TYPE::STATIC) {
        std::cerr << "The buffer is static!" << std::endl;
        return;
    }

    Bind();

    if (m_Capacity < size) {
        m_Capacity = size;
        gapi.BufferData(BUFFER_TYPE::ARRAY, m_Capacity, data, m_BufferType);
    } else {
        gapi.BufferSubData(BUFFER_TYPE::ARRAY, 0, size, data);
    }
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer &&other) {
    m_Capacity   = other.m_Capacity;
    m_ID         = other.m_ID;
    m_BufferType = other.m_BufferType;

    other.m_Capacity   = 0;
    other.m_ID         = 0;

    return *this;
}

void VertexBuffer::Bind() const {
    gapi.BindBuffer(BUFFER_TYPE::ARRAY, m_ID);
}

void VertexBuffer::UnBind() const {
    gapi.BindBuffer(BUFFER_TYPE::ARRAY, 0);
}

VertexBuffer::~VertexBuffer() {
    UnBind();
    gapi.DeleteBuffers(1, &m_ID);
}
