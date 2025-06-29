#include "glib.h"

glib::Shader::Shader(const char* filePath) {
    std::string customShaderSrc = GlCore::Cache.GetTemplate();
    customShaderSrc += '\n';
    customShaderSrc.append(GlCore::ShaderSourceLoader::Parse(filePath));
//    std::cout << customShaderSrc << std::endl;
    m_CustomShader.LoadFromString(customShaderSrc);

}

GlCore::ShaderProgram &glib::Shader::GetShader() {
    return m_CustomShader;
}
