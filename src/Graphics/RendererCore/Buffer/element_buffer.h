#pragma once

#include "Graphics/GraphicsAPI/graphics_api.h"

namespace RendererCore {
    class ElementBuffer {
    public:
        ElementBuffer(GAPI::DRAW_TYPE drawType, uint32_t count, const void* data);
        ~ElementBuffer();

        ElementBuffer& operator=(ElementBuffer&& other);

        void PutData(const void* data, uint32_t count);
        void Bind() const;
        void UnBind() const;
        uint32_t GetCount() const;
    private:
        uint32_t m_Count = 0;
        
        GAPI::DRAW_TYPE m_BufferType;
        uint32_t m_Capacity = 0;
        uint32_t m_ID = 0;
    };
}
