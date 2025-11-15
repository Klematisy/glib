#pragma once

#include "Graphics/GraphicsAPI/graphics_api_impl.h"

namespace RendererCore {
    class VertexBuffer {
    public:
        VertexBuffer() = default;
        ~VertexBuffer();
        VertexBuffer(GAPI::DRAW_TYPE bufferType, uint32_t size, const void* data);

        VertexBuffer& operator=(VertexBuffer&& other);

        void PutData(uint32_t size, const void* data);
        void Bind() const;
        void UnBind() const;
    private:
        GAPI::DRAW_TYPE m_BufferType;
        uint32_t m_Capacity = 0;
        uint32_t m_ID = 0;
    };
}