#pragma once

#include "Shader/ShaderProgram.h"

#include <vector>

namespace GlCore {

    class ShaderCache {
    public:
        typedef std::vector<std::pair<ShaderProgram *, const char *>> _shaderVecDB;

        static ShaderCache &GetCache();

        void LoadCache();

        const std::string &GetTemplate();

        ShaderProgram &GetBasicProgram();

        void AddCustomShader(ShaderProgram *program, const char *filePath);

        void AddBasicShader(ShaderProgram *program, const char *filePath);

        void DeleteGarbageElement(_shaderVecDB &db, uint32_t i);

        void HotReload();

    private:
        ShaderProgram m_BasicShader;
        std::string m_ShaderTemplate;

        _shaderVecDB m_BasicShaderDataBase;
        _shaderVecDB m_CustomShaderDataBase;

        ShaderCache() = default;

        static ShaderCache s_CacheInstance;
    };

    inline ShaderCache ShaderCache::s_CacheInstance;

}