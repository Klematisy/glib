#pragma once

#include "GL/glew.h"

#include "vertex_array_layout.h"
#include "vertex_buffer.h"

namespace GlCore {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();
        VertexArray& operator=(VertexArray &&other);

        void Bind() const;
        void UnBind() const;
        void AddBuffer(const VertexArrayLayout &layout, const VertexBuffer &vb);
    private:
        uint32_t m_ID = 0;
    };

}
