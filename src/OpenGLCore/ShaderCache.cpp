#include "ShaderCache.h"


void GlCore::ShaderCache::LoadCache() {
    m_ShaderTemplate = ShaderSourceLoader::Parse("resources/shaders/template.glsl");
    m_BasicShader.LoadFromFile("resources/shaders/base_shader.glsl");
    m_ShaderDataBase.push_back({ShaderTemplateType::NONE, &m_BasicShader, "resources/shaders/base_shader.glsl"});
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

void GlCore::ShaderCache::DeleteGarbageElement() {
    for (int i = 0; i < m_ShaderDataBase.size(); i++) {
        if (!m_ShaderDataBase[i].program)
            m_ShaderDataBase.erase(m_ShaderDataBase.cbegin() + i);
    }
}

void GlCore::ShaderCache::HotReload() {
    DeleteGarbageElement();

    for (auto &element : m_ShaderDataBase) {
        if (element.loadType == ShaderTemplateType::WITH_TEMPLATE) {
            std::string src = m_ShaderTemplate;
            src.append(ShaderSourceLoader::Parse(element.filePath));
            element.program->LoadFromString(src);
        } else {
            element.program->LoadFromFile(element.filePath);
        }
    }

    using namespace std::string_literals;
    Logger::LogInf("SHADER", "HOT RELOAD {"s + std::to_string(m_HotReloadCount) + "}");

    m_HotReloadCount++;
}

void GlCore::ShaderCache::AddShader(ShaderTemplateType loadType, GlCore::ShaderProgram *program, const char *filePath) {
    m_ShaderDataBase.push_back({loadType, program, filePath});
}
