#include "vertex_array.h"
#include "../renderer.h"
#include "Graphics/RendererCore/Buffer/element_buffer.h"

using namespace RendererCore;
using namespace GAPI;
static const auto gapi = rendererAPI;

VertexArray::VertexArray() {
    gapi->CreateVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray() {
    gapi->DeleteVertexArrays(1, &m_ID);
}

VertexArray& VertexArray::operator=(VertexArray&& other) {
    m_ID = other.m_ID;
    other.m_ID = 0;

    return *this;
}

void VertexArray::Bind() const {
    gapi->BindVertexArray(m_ID);
}

void VertexArray::UnBind() const {
    gapi->BindVertexArray(0);
}

void VertexArray::AddElementBuffer(const ElementBuffer& vb) {
    Bind();
    vb.Bind();

    UnBind();
    vb.UnBind();
}

void VertexArray::AddVertexBuffer(const VertexArrayLayout& layout, const VertexBuffer& vb) {
    const auto& layouts = layout.GetLayouts();

    Bind();
    vb.Bind();

    int fullOffset = layout.GetFullOffset();

    uint32_t i = 0;
    for (const LayoutData &element : layouts) {

        gapi->VertexAttribPointer(i, (int) element.size, element.type, API_BOOLEAN::FALSE,  fullOffset, (void*) element.offset);
        gapi->EnableVertexAttribArray(i++);
    }

    UnBind();
    vb.UnBind();
}
