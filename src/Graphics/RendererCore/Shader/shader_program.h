#pragma once

#include <unordered_map>

#include "shader.h"
#include "Graphics/GraphicsAPI/graphics_api.h"
#include "glm/glm.hpp"

namespace RendererCore {

    class ShaderProgram {
    public:
        ShaderProgram();
        ~ShaderProgram();

        ShaderProgram& operator=(const ShaderProgram& other);
        ShaderProgram& operator=(ShaderProgram&& other);

        void Bind() const;
        void UnBind() const;

        void AttachShader(const Shader& shader);
        void LinkProgram();
        void ClearShaders();

        //-----------------------------UNIFORMS-----------------------------//
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetFloat2(const std::string& name, const glm::vec2& val) const;
        void SetIntArray(const std::string& name, uint32_t count, const int* value) const;
        void SetMatrixFloat4(const std::string& name, const float *value_ptr) const;
        //-----------------------------UNIFORMS-----------------------------//

    private:
        int GetUniformLocation(const std::string& name) const;
        void CheckLinkingErrors() const;
    private:
        std::vector<const Shader*> m_AttachedShaders;
        mutable std::unordered_map<std::string, int> m_UniformLocations;
        GAPI::GraphicsObject m_ShaderProgram = 0;
    };

}
