#include "ShaderCache.h"


void GlCore::ShaderCache::LoadCache() {
    m_ShaderTemplate = ShaderSourceLoader::Parse("resources/shaders/template.glsl");
    m_BasicShader.LoadFromFile("resources/shaders/base_shader.glsl");
    AddBasicShader(&m_BasicShader, "resources/shaders/base_shader.glsl");
}

const std::string &GlCore::ShaderCache::GetTemplate() {
    return m_ShaderTemplate;
}

GlCore::ShaderProgram &GlCore::ShaderCache::GetBasicProgram() {
    return m_BasicShader;
}

GlCore::ShaderCache &GlCore::ShaderCache::GetCache() {
    return s_CacheInstance;
}

void GlCore::ShaderCache::DeleteGarbageElement(_shaderVecDB& db, uint32_t i) {
    if (!db[i].first)
        db.erase(db.cbegin() + i);
}

void GlCore::ShaderCache::HotReload() {
    uint32_t i = 0;

    while (i < m_BasicShaderDataBase.size()) {
        DeleteGarbageElement(m_BasicShaderDataBase, i);

        if (i == m_CustomShaderDataBase.size()) break;

        m_BasicShaderDataBase[i].first->LoadFromFile(m_BasicShaderDataBase[i].second);
        i++;
    }

    std::cout << "SHADER: HOT RELOADED" << std::endl;

    i = 0;

    while (i < m_CustomShaderDataBase.size()) {
        DeleteGarbageElement(m_CustomShaderDataBase, i);

        if (i == m_CustomShaderDataBase.size()) break;

        std::string src = m_ShaderTemplate;
        src.append(GlCore::ShaderSourceLoader::Parse(m_CustomShaderDataBase[i].second));
        m_CustomShaderDataBase[i].first->LoadFromString(src);
        i++;
    }

    std::cout << "CUSTOM_SHADER: HOT RELOADED" << std::endl;
}



void GlCore::ShaderCache::AddCustomShader(GlCore::ShaderProgram *program, const char* filePath) {
    m_CustomShaderDataBase.emplace_back(program, filePath);
}

void GlCore::ShaderCache::AddBasicShader(GlCore::ShaderProgram *program, const char *filePath) {
    m_BasicShaderDataBase.emplace_back(program, filePath);
}
