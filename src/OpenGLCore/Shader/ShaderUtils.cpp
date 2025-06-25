#include "ShaderUtils.h"

std::string ShaderSourceLoader::Parse(const std::string &filePath) {
    std::ifstream file("resources/shaders/base_shader.glsl");
    std::string shader_file;

    if (file.is_open()) {
        std::string ch;
        while(getline(file, ch)) {
            shader_file.append(ch + "\n");
        }
    } else {
        std::cerr << "FILE ISN'T OPEN!!" << std::endl;
    }
    file.close();

    return std::move(shader_file);
}

ShaderCreator::ShaderCreator(const std::string &filePath) {
    m_ShaderSrc = ShaderSourceLoader::Parse(filePath);
}

uint32_t ShaderCreator::CreateShader(uint32_t shader_type) {
    const char* shader_specify;
    switch (shader_type) {
        case GL_VERTEX_SHADER:   shader_specify = "VERTEX";    break;
        case GL_FRAGMENT_SHADER: shader_specify = "FRAGMENT";  break;
        default:                 shader_specify = "UNKNOWN";
    }

    std::string fp = "#define SHADER_";
    fp += shader_specify;
    fp += " 1 \n";
    const char* specified_shader[] = {
            "#version 410 core\n",
            fp.c_str(),
            m_ShaderSrc.c_str()
    };
    uint32_t shader = glCreateShader(shader_type);
    glShaderSource(shader, 3, specified_shader, nullptr);
    glCompileShader(shader);

    CheckShaderErrors(shader, shader_specify);

    return shader;
}

void ShaderCreator::CheckShaderErrors(uint32_t shader, const char* specified_shader) {
    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);
        std::cerr << "Failed to compile " << specified_shader << " shader!" << std::endl;
        std::cerr << message << std::endl;
        free(message);
    }
}

void ShaderCreator::CheckLinkingErrors(uint32_t shaderProgram) {
    int result;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetProgramInfoLog(shaderProgram, length, nullptr, message);
        std::cerr << "Failed to link program!" << std::endl;
        std::cerr << message << std::endl;
        free(message);
    }
}
