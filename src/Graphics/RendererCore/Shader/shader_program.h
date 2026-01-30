#pragma once

#include <unordered_map>

#include "shader.h"

namespace RendererCore {

    class ShaderProgram {
    public:
        ShaderProgram() = default;
        ~ShaderProgram();

        ShaderProgram& operator=(const ShaderProgram& other);
        ShaderProgram& operator=(ShaderProgram&& other);

        void Bind() const;
        void UnBind() const;

        void AttachShader(const Shader& shader);
        void ClearShaders();
        void CreateProgram();

        void SetInt(const std::string& name, int value);
        void SetFloat(const std::string& name, float value);
        void SetIntArray(const std::string& name, uint32_t count, const int* value);
        void SetMatrixFloat4(const std::string& name, const float *value_ptr);
    private:
        int GetUniformLocation(const std::string& name);
        void CheckLinkingErrors() const;
    private:
        std::vector<uint32_t> m_AttachedShaders;
        std::unordered_map<std::string, int> UniformLocations;
        uint32_t m_ShaderProgram = 0;
    };

}