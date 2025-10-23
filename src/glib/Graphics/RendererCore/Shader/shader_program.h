#pragma once

#include <unordered_map>
#include "GL/glew.h"

#include "shader.h"

namespace RendererCore {

    class ShaderProgram {
    public:
        ShaderProgram() = default;
        ~ShaderProgram();

        ShaderProgram& operator=(const ShaderProgram& other);
        ShaderProgram& operator=(ShaderProgram &&other);

        void Bind() const;
        void UnBind() const;

        void AttachShader(const Shader& shader);
        void ClearShaders();
        void CreateProgram();

        void SetUniform1i(const std::string &name, int value);
        void SetUniform1f(const std::string &name, float value);
        void SetUniform1iv(const std::string &name, uint32_t count, const int *value);
        void SetUniformMatrix4fv(const std::string &name, const float *value_ptr);
    private:
        int GetUniformLocation(const std::string &name);
        void CheckLinkingErrors() const;
    private:
        std::vector<uint32_t> m_AttachedShaders;
        std::unordered_map<std::string, int> UniformLocations;
        uint32_t m_ShaderProgram = 0;
    };

}