#include "glib.h"

glib::Shader::Shader(const char* filePath) {
    using namespace GlCore;
    using namespace std::string_literals;

    std::string customShaderSrc = ShaderCache::GetCache().GetTemplate();
    customShaderSrc += '\n';
    customShaderSrc.append(ShaderSourceLoader::Parse(filePath));
    if (m_CustomShader.LoadFromString(customShaderSrc) == -1) {
        Logger::LogErr("CUSTOM SHADER", "'"s + filePath + "' hasn't loaded");
    } else {
        Logger::LogInf("CUSTOM SHADER", "'"s + filePath + "' has loaded");
    }
    ShaderCache::GetCache().AddShader(ShaderCache::ShaderTemplateType::WITH_TEMPLATE, &m_CustomShader, filePath);
}

GlCore::ShaderProgram &glib::Shader::GetShader() {
    return m_CustomShader;
}