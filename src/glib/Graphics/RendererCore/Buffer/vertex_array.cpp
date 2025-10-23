#include <iostream>

#include "vertex_array.h"

using namespace RendererCore;
using namespace GAPI;

static GraphicsAPIImpl& gapi = GraphicsAPIImpl::Get();

VertexArray::VertexArray() {
    gapi.CreateVertexArrays(1, &m_ID);
    Bind();
}

VertexArray::~VertexArray() {
    gapi.DeleteVertexArrays(1, &m_ID);
}

VertexArray& VertexArray::operator=(VertexArray &&other) {
    m_ID = other.m_ID;
    other.m_ID = 0;

    return *this;
}

void VertexArray::Bind() const {
    gapi.BindVertexArray(m_ID);
}

void VertexArray::UnBind() const {
    gapi.BindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexArrayLayout &layout, const VertexBuffer &vb) {
    const auto& layouts = layout.GetLayouts();

    Bind();
    vb.Bind();

    int fullOffset = layout.GetFullOffset();

    uint32_t i = 0;
    for (const LayoutData &element : layouts) {

        gapi.VertexAttribPointer(i, (int) element.size, element.type, API_BOOLEAN::FALSE,  fullOffset, (void*) element.offset);
        gapi.EnableVertexAttribArray(i++);
    }
}


