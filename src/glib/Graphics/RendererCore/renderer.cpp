#include "renderer.h"

using namespace RendererCore;
using namespace GAPI;

static GraphicsAPIImpl& gapi = GraphicsAPIImpl::Get();

void Renderer::Draw(const ShaderProgram& shader, const VertexArray& va, const ElementBuffer& eb) {
    va.Bind();
    eb.Bind();
    shader.Bind();
    gapi.DrawElements(RENDERER_TYPE::TRIANGLES, static_cast<int>(eb.GetCount()), API_TYPE::UINT, nullptr);
}

void Renderer::Clear() {
    gapi.Clear(CLEAR_BUFFER_BIT::COLOR, CLEAR_BUFFER_BIT::DEPTH);
}
