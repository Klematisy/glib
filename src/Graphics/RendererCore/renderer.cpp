#include "renderer.h"

using namespace RendererCore;
using namespace GAPI;
static const auto gapi = rendererAPI;

void Renderer::Draw(const RenderItem& item) {
    const auto& va = *item.vertexArray;
    const auto& eb = *item.elementBuffer;

    va.Bind();
    item.shader->Bind();
    item.texture->Bind();

    gapi->DrawElements(item.renderType, static_cast<int>(eb.GetCount()), API_TYPE::UINT, nullptr);

    va.UnBind();
    item.shader->UnBind();
    item.texture->UnBind();

    m_Stats.drawCalls++;
    m_Stats.triangles += eb.GetCount();
}

void Renderer::Clear() {
    gapi->Clear(CLEAR_BUFFER_BIT::COLOR | CLEAR_BUFFER_BIT::DEPTH);
}

RenderStats Renderer::GetStats() const {
    RenderStats r = m_Stats;
    m_Stats = {};
    return r;
}

//void Renderer::DrawEdges(bool de) {
//    m_DrawEdges = {};
//    m_DrawEdges = de;
//}

//void Renderer::DrawLines(const ShaderProgram &shader, const VertexArray &va, const ElementBuffer &eb) {
//    va.Bind();
//    eb.Bind();
//    shader.Bind();
//
//    // TODO: RAW OPENGL
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    gapi->DrawElements(RENDERER_TYPE::TRIANGLES, static_cast<int>(eb.GetCount()), API_TYPE::UINT, nullptr);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//}
