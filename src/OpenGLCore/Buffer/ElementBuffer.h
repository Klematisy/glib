#pragma once

#include "GL/glew.h"

namespace GlCore {

    class ElementBuffer {
    public:
        ElementBuffer() = default;

        ElementBuffer(uint32_t bufferType, uint32_t count, const void *data);

        ElementBuffer& operator=(ElementBuffer &&other);

        void PutData(uint32_t count, const void *data);

        void Bind() const;

        void UnBind() const;

        uint32_t GetCount() const;

        ~ElementBuffer();

    private:
        uint32_t m_Count = 0;
        uint32_t m_Capacity = 0;
        uint32_t m_BufferType = 0;
        uint32_t m_ID = 0;
    };

}