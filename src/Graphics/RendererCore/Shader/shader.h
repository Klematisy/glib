#pragma once

#include <string>
#include <unordered_map>
#include <cinttypes>

#include "Graphics/GraphicsAPI/graphics_api_opengl.h"

namespace RendererCore {
    class Shader {
    public:
        Shader() = default;
        ~Shader();

        void SetShaderSourceFile(const char* filePath);
        void Compile();
        void PreProcess();

        std::vector<uint32_t> GetShaders() const;
        void DeleteShader();
    private:
        std::string GetShaderDefine(GAPI::SHADER_TYPE shader_type);
        uint32_t CheckShaderErrors(uint32_t shader);
        bool IsEqualDirective(const std::string& directive, uint32_t index);
        void DefineShader();

        uint32_t m_Id = 0;

        std::unordered_map<GAPI::SHADER_TYPE, uint32_t> m_Shaders;
        std::string m_Src;
        std::string m_FileEnvironment;
        std::string m_SType;
    };
}
