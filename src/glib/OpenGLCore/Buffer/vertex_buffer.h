#pragma once

#include "GL/glew.h"

namespace GlCore {

    class VertexBuffer {
    public:
        VertexBuffer() = default;

        VertexBuffer(uint32_t bufferType, uint32_t size, const void *data);

        VertexBuffer& operator=(VertexBuffer &&other);

        void PutData(uint32_t size, const void *data);

        void Bind() const;

        void UnBind() const;

        ~VertexBuffer();

    private:
        uint32_t m_Capacity = 0;
        uint32_t m_BufferType = 0;
        uint32_t m_ID = 0;
    };

}