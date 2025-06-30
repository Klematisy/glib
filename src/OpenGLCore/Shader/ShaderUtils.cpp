#include "ShaderUtils.h"

std::string GlCore::ShaderSourceLoader::Parse(const char* filePath) {
    std::ifstream file(filePath);
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

int GlCore::ShaderCreator::CreateShader(const std::string &m_ShaderSrc, uint32_t shader_type) {
    const char* shader_specify;
    switch (shader_type) {
        case GL_VERTEX_SHADER:   shader_specify = "VERTEX";    break;
        case GL_FRAGMENT_SHADER: shader_specify = "FRAGMENT";  break;
        default:                 shader_specify = "UNKNOWN";
    }

    std::string define_shader = "#define SHADER_";
    define_shader += shader_specify;
    define_shader += '\n';

    std::string max_slots_count = "#define MAX_SLOTS_COUNT ";
    max_slots_count += std::to_string(GetMaxSlotsCount());
    max_slots_count += '\n';

    const char* specified_shader[] = {
            "#version 410 core\n",
            max_slots_count.c_str(),
            define_shader.c_str(),
            m_ShaderSrc.c_str()
    };

//    std::cout << specified_shader[0] << std::endl;
//    std::cout << specified_shader[1] << std::endl;
//    std::cout << specified_shader[2] << std::endl;
//    std::cout << specified_shader[3] << std::endl;

    uint32_t shader = glCreateShader(shader_type);
    glShaderSource(shader, 4, specified_shader, nullptr);
    glCompileShader(shader);

    if (CheckShaderErrors(shader, shader_specify) == -1) {
        return -1;
    }

    return shader;
}

int GlCore::ShaderCreator::CheckShaderErrors(uint32_t shader, const char* specified_shader) {
    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);
        std::cerr << "\nFailed to compile " << specified_shader << " shader!" << std::endl;
        std::cerr << message << std::endl;
        free(message);
        return -1;
    }
    return 0;
}

int GlCore::ShaderCreator::CheckLinkingErrors(uint32_t shaderProgram) {
    int result;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetProgramInfoLog(shaderProgram, length, nullptr, message);
        std::cerr << "\nFailed to link program!" << std::endl;
        std::cerr << message << std::endl;
        free(message);
        return -1;
    }
    return 0;
}

int GlCore::ShaderCreator::GetMaxSlotsCount() {
    int maxTextureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    return maxTextureUnits;
}
