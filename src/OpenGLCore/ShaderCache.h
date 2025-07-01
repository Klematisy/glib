#pragma once

#include "Shader/ShaderProgram.h"

#include <vector>

namespace GlCore {

    class ShaderCache {
    public:
        enum class SHADER_TEMPLATE {WITHOUT, USE};

        static ShaderCache &GetCache();

        void LoadCache();
        const std::string &GetTemplate();
        ShaderProgram &GetBasicProgram();
        void AddShader(SHADER_TEMPLATE loadType, ShaderProgram *program, const char *filePath);
        void DeleteGarbageElement();
        void HotReload();

    private:
        struct DBElement {
            SHADER_TEMPLATE loadType = SHADER_TEMPLATE::WITHOUT;
            ShaderProgram* program = nullptr;
            const char* filePath = "";
        };

        std::vector<DBElement> m_ShaderDataBase;

        ShaderProgram m_BasicShader;
        std::string m_ShaderTemplate;

        ShaderCache() = default;

        static ShaderCache s_CacheInstance;
    };

    inline ShaderCache ShaderCache::s_CacheInstance;

}