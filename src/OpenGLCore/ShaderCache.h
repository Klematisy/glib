#pragma once

#include "Shader/ShaderProgram.h"
#include "Logger.h"

#include <vector>
#include <string>

namespace GlCore {

    class ShaderCache {
    public:
        enum class ShaderTemplateType { NONE, WITH_TEMPLATE };

        static ShaderCache &GetCache();

        void LoadCache();
        const std::string &GetTemplate();
        ShaderProgram &GetBasicProgram();
        void AddShader(ShaderTemplateType loadType, ShaderProgram *program, const char *filePath);
        void DeleteGarbageElement();
        void HotReload();

    private:
        uint32_t m_HotReloadCount = 0;

        struct DBElement {
            ShaderTemplateType loadType = ShaderTemplateType::NONE;
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