#pragma once

#include <string>
#include <unordered_map>
#include <cinttypes>

namespace GlCore {
    class Shader {
    public:
        Shader() = default;
        ~Shader();

        void SetShaderSourceFile(const char *filePath);
        void Compile();
        void PreProcess();

        std::vector<uint32_t> GetShaders() const;
        void DeleteShader();
    private:
        std::string GetDefineShader(uint32_t shader_type);
        uint32_t CheckShaderErrors(uint32_t shader);
        bool IsEqualDirective(const std::string &directive, uint32_t index);
        void DefineShader();

        uint32_t m_Id = 0;

        std::unordered_map<uint32_t, uint32_t> m_Shaders;
        std::string m_Src;
        std::string m_FileEnvironment;
        std::string m_SType;
    };
}