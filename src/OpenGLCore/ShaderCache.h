#pragma once

#include "Shader/ShaderProgram.h"

#include <vector>

namespace GlCore {

    class ShaderCache {
    public:
        static ShaderCache &GetCache();

        static enum class TEMPLATE {WITHOUT, USE};

        void LoadCache();
        const std::string &GetTemplate();
        ShaderProgram &GetBasicProgram();
        void AddShader(TEMPLATE loadType, ShaderProgram *program, const char *filePath);
        void DeleteGarbageElement();
        void HotReload();

    private:
        struct DBElement {
            TEMPLATE loadType = TEMPLATE::WITHOUT;
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