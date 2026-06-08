#pragma once

#include <string>
#include <vector>

#include "Graphics/GraphicsAPI/graphics_api.h"

namespace RendererCore {
    std::string ParseFile(const std::string& filePath);
    std::string getShaderTypeInStr(GAPI::SHADER_TYPE type);

    class Shader {
        friend class ShaderCompiler;
        friend class ShaderProgram;
    public:
        Shader(Shader&& other) noexcept;
        Shader& operator=(Shader&& other) noexcept;
        
        Shader(std::string filePath, GAPI::SHADER_TYPE shaderType);
        ~Shader();
    private:
        uint32_t m_Id = 0;
        std::string m_FilePath;
        GAPI::SHADER_TYPE m_ShaderType;
    };

    class ShaderCompiler {
    public:
        std::vector<std::string> precompiledOptions;

        void Compile(Shader& shader);
    private:
        std::string PreProcess(std::string src, std::string filePath);
        void CheckShaderErrors(Shader& shader);
    };
}
