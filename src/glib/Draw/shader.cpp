#include "shader.h"

using namespace glib;

using glcore_sp = GlCore::ShaderProgram;

void Shader::HotReload() {
    m_Program.reset();
    m_Program = std::make_shared<GlCore::ShaderProgram>();
    m_AddedCount = m_FilePaths.size();
    Compile();
}

void Shader::Compile() {
    if (m_AddedCount == 0) return;

    std::vector<GlCore::Shader> shader;
    shader.reserve(m_AddedCount);

    uint32_t size = m_FilePaths.size();
    for (uint32_t i = size - m_AddedCount; i < size; i++) {
        shader.emplace_back();
        auto &lsh = shader.back();

        lsh.SetShaderSourceFile(m_FilePaths[i]);
        lsh.PreProcess();
        lsh.Compile();

        m_Program->AttachShader(lsh);
    }

    m_Program->CreateProgram();
}

std::shared_ptr<glcore_sp> Shader::GetShaderProgram() {
    return m_Program;
}
