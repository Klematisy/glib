#include "graphics_api_opengl.h"
#include "type_casting.h"

void RendererOpenGL::Draw(const RenderItem& item) {
    const auto& va = *item.vertexArray;
    const auto& eb = *item.elementBuffer;

    va.Bind();
    item.shader->Bind();
    item.texture->Bind();

    glDrawElements(GL::RENDERER_TYPE[TO_INT(item.renderType)], static_cast<int>(eb.GetCount()), GL::ConvertAPITypeToGlType(API_TYPE::UINT), nullptr);

    va.UnBind();
    item.shader->UnBind();
    item.texture->UnBind();

    m_Stats.drawCalls++;
    m_Stats.triangles += eb.GetCount();
}

void RendererOpenGL::Clear(CLEAR_BUFFER_BIT bits) {
    GLbitfield result = 0;

    if ((static_cast<int>(bits) & static_cast<uint32_t>(CLEAR_BUFFER_BIT::COLOR)) != 0) {
        result |= GL_COLOR_BUFFER_BIT;
    }
    if ((static_cast<int>(bits) & static_cast<uint32_t>(CLEAR_BUFFER_BIT::DEPTH)) != 0) {
        result |= GL_DEPTH_BUFFER_BIT;
    }
    if ((static_cast<int>(bits) & static_cast<uint32_t>(CLEAR_BUFFER_BIT::STENCIL)) != 0) {
        result |= GL_STENCIL_BUFFER_BIT;
    }

    glClear(result);
}

RenderStats RendererOpenGL::GetStats() const {
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
