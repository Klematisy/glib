#include "glib.h"

glib::Shader::Shader(const char* filePath) {
    std::string customShaderSrc = GlCore::Cache.GetTemplate();
    customShaderSrc += '\n';
    customShaderSrc.append(GlCore::ShaderSourceLoader::Parse(filePath));
    if (m_CustomShader.LoadFromString(customShaderSrc) == -1) {
        std::cerr << "CUSTOM_SHADER: '" << filePath << "' hasn't loaded" << std::endl;
    } else {
        std::cout << "CUSTOM_SHADER: '" << filePath << "' has loaded" << std::endl;
    }
}

GlCore::ShaderProgram &glib::Shader::GetShader() {
    return m_CustomShader;
}
