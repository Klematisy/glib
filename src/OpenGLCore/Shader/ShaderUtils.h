
#include <iostream>
#include <fstream>
#include "GL/glew.h"

class ShaderSourceLoader {
public:
    static std::string Parse(const std::string& filePath);
};

class ShaderCreator {
public:
    explicit ShaderCreator(const std::string& filePath);
    uint32_t CreateShader(uint32_t type);
    static void CheckShaderErrors(uint32_t shader, const char* specified_shader);
    static void CheckLinkingErrors(uint32_t shader);
private:
    std::string m_ShaderSrc;
};