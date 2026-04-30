#include "shader.h"

VLADLIB_NAMESPACE_USING;

using glcore_sp = RendererCore::ShaderProgram;

void Shader::HotReload() {
    m_Program.reset();
    m_Program = std::make_shared<RendererCore::ShaderProgram>();
    m_AddedCount = m_FilePaths.size();
    Compile();
}

void Shader::Compile() {
    if (m_AddedCount == 0) return;

    std::vector<RendererCore::Shader> shader;
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

glcore_sp* Shader::GetShaderProgram() const {
    return m_Program.get();
}

bool Shader::IsValid() const {
    return m_Program.get();
}
