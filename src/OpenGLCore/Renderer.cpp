#include "Renderer.h"

void GlCore::Renderer::Draw(const ShaderProgram& shader, const VertexArray& va, const ElementBuffer& eb) {
    va.Bind();
    shader.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(eb.GetCount()), GL_UNSIGNED_INT, nullptr);
}

void GlCore::Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void GlCore::ShaderCache::LoadCache() {
    m_ShaderTemplate = ShaderSourceLoader::Parse("resources/shaders/template.glsl");
    m_BasicShader.LoadFromFile("resources/shaders/base_shader.glsl");
}

const std::string &GlCore::ShaderCache::GetTemplate() {
    return m_ShaderTemplate;
}

GlCore::ShaderProgram &GlCore::ShaderCache::GetBasicProgram() {
    return m_BasicShader;
}