#include "renderer.h"

using namespace RendererCore;
using namespace GAPI;

void Renderer::Draw(const GraphicsBuffer& gb, const ShaderProgram& shader) {
    auto& va = *gb.vertexArray;
    auto& eb = *gb.elementBuffer;

    va.Bind();
    eb.Bind();
    shader.Bind();
    gapi.DrawElements(m_RendererType, static_cast<int>(eb.GetCount()), API_TYPE::UINT, nullptr);
}

void Renderer::Clear() {
    gapi.Clear(CLEAR_BUFFER_BIT::COLOR, CLEAR_BUFFER_BIT::DEPTH);
}

void Renderer::SetRendererType(GAPI::RENDERER_TYPE rendererType) {
    m_RendererType = rendererType;
}

void Renderer::DrawEdges(bool de) {
    m_DrawEdges = de;
}

Renderer::Renderer(GAPI::RENDERER_TYPE rendererType, bool DrawEdges) {
    m_RendererType = rendererType;
    m_DrawEdges = DrawEdges;
}

//void Renderer::DrawLines(const ShaderProgram &shader, const VertexArray &va, const ElementBuffer &eb) {
//    va.Bind();
//    eb.Bind();
//    shader.Bind();
//
//    // TODO: RAW OPENGL
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    gapi.DrawElements(RENDERER_TYPE::TRIANGLES, static_cast<int>(eb.GetCount()), API_TYPE::UINT, nullptr);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//}
