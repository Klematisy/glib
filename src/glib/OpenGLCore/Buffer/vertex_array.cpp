#include <iostream>

#include "vertex_array.h"

GlCore::VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_ID);
    Bind();
}

GlCore::VertexArray::~VertexArray() {
    if (m_ID != 0)
        glDeleteVertexArrays(1, &m_ID);
}

GlCore::VertexArray& GlCore::VertexArray::operator=(GlCore::VertexArray &&other) {
    m_ID = other.m_ID;

    other.m_ID = 0;

    return *this;
}

void GlCore::VertexArray::Bind() const {
    glBindVertexArray(m_ID);
}

void GlCore::VertexArray::UnBind() const {
    glBindVertexArray(0);
}

void GlCore::VertexArray::AddBuffer(const VertexArrayLayout &layout, const VertexBuffer &vb) {
    const auto& layouts = layout.GetLayouts();

    Bind();
    vb.Bind();

    int fullOffset = layout.GetFullOffset();

    uint32_t i = 0;
    for (const LayoutData &element : layouts) {

        glVertexAttribPointer(i, (int) element.size, element.type, GL_FALSE,  fullOffset, (void*) element.offset);
        glEnableVertexAttribArray(i++);
    }
}


