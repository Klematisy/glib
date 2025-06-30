#pragma once

#include <unordered_map>
#include "GL/glew.h"

#include "ShaderUtils.h"

namespace GlCore {

    class ShaderProgram {
    public:
        ShaderProgram() = default;

        ShaderProgram& operator=(const ShaderProgram& other);

        ~ShaderProgram();

        ShaderProgram& operator=(ShaderProgram &&other);

        void LoadFromFile(const char* filePath);

        void LoadFromString(const std::string& SourceCode);

        void Bind() const;

        void UnBind() const;

        void SetUniform1i(const std::string &name, int value);

        void SetUniform1f(const std::string &name, float value);

        void SetUniform1iv(const std::string &name, uint32_t count, const int *value);

        void SetUniformMatrix4fv(const std::string &name, const float *value_ptr);

    private:
        void CreateShaderProgram(const std::string& ShaderSourceCode);
        int GetUniformLocation(const std::string &name);
    private:
        std::unordered_map<std::string, int> UniformLocations;
        uint32_t m_ShaderProgram = 0;
    };

}