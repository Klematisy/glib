#include "shader.h"

using namespace glib;

using glcore_sp = GlCore::ShaderProgram;

void Shader::HotReload() {
    // TODO: Hot reload
}

void Shader::Compile() {
    // TODO: Должна быть проверка на необходимость компиляции

    std::vector<GlCore::Shader> shader;
    shader.reserve(m_AddedCount);

    uint32_t size = m_FilePaths.size();
    for (uint32_t i = size - m_AddedCount; i < size; i++) {
        uint32_t j = i - size + m_AddedCount;
        shader.emplace_back();

        shader[j].SetShaderSourceFile(m_FilePaths[i]);
        shader[j].PreProcess();
        shader[j].Compile();

        m_Program->AttachShader(shader[j]);
    }

    m_Program->CreateProgram();
}

std::shared_ptr<glcore_sp> Shader::GetShaderProgram() {
    return m_Program;
}