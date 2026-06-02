#pragma once

#include "Graphics/RendererCore/Buffer/element_buffer.h"
#include "vertex_array_layout.h"
#include "vertex_buffer.h"

namespace RendererCore {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();
        VertexArray& operator=(VertexArray&& other);

        void Bind() const;
        void UnBind() const;
        void AddElementBuffer(const ElementBuffer& vb);
        void AddVertexBuffer(const VertexArrayLayout& layout, const VertexBuffer& vb);
    private:
        uint32_t m_ID = 0;
    };
}
