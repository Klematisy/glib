#include "renderer.h"

void GlCore::Renderer::Draw(const ShaderProgram& shader, const VertexArray& va, const ElementBuffer& eb) {
    va.Bind();
    eb.Bind();
    shader.Bind();
    GlCall(glDrawElements(GL_TRIANGLES, static_cast<int>(eb.GetCount()), GL_UNSIGNED_INT, nullptr));
}

void GlCore::Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
