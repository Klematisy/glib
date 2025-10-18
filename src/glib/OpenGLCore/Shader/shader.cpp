#include <fstream>
#include <filesystem>

#include "Logger/logger.h"
#include "shader.h"
#include "GL/glew.h"
#include "preprocessor.h"

using namespace GlCore;

static void ParseFile(const char* filePath, std::string& src) {
    std::ifstream file(filePath);
    using namespace std::string_literals;
    std::string ch;

    if (!file.is_open()) {
        Logger::LogWar("SHADER", "File '"s + filePath + "' isn't open!");
    }

    while (getline(file, ch)) {
        src.append(ch + '\n');
    }
}


void Shader::SetShaderSourceFile(const char *filePath) {
    ParseFile(filePath, m_Src);
    m_FileEnvironment = std::filesystem::path(filePath).string();
}

std::string Shader::GetDefineShader(uint32_t shader_type) {
    switch (shader_type) {
        case GL_VERTEX_SHADER:   m_SType = "VERTEX";   break;
        case GL_FRAGMENT_SHADER: m_SType = "FRAGMENT"; break;
        case GL_COMPUTE_SHADER:  m_SType = "COMPUTE";  break;
        case GL_GEOMETRY_SHADER: m_SType = "GEOMETRY"; break;
        default:                 m_SType = "UNKNOWN";
    }

    std::string define_shader = "#define SHADER_";
    define_shader.append(m_SType);
    define_shader += '\n';

    return define_shader;
}

void Shader::Compile() {
    for (auto& it : m_Shaders) {
        std::string define_shader = GetDefineShader(it.first);

        // TODO: max slots count fix

        const char *specified_shader[] = {
                "#version 410 core\n",  // TODO: flexible version
                define_shader.c_str(),
                m_Src.c_str()
        };

        m_Id = glCreateShader(it.first);
        glShaderSource(m_Id, 3, specified_shader, nullptr);
        glCompileShader(m_Id);

        if (CheckShaderErrors(m_Id) == -1) return;

        m_Shaders[it.first] = m_Id;
    }
}

bool Shader::IsEqualDirective(const std::string& directive, uint32_t index) {
    bool equal = true;
    for (uint32_t k = 0; k < directive.size(); k++) {
        if (m_Src[index + k] != directive[k]) {
            equal = false;
            break;
        }
    }
    return equal;
}

void Shader::DefineShader() {
    std::string startDir = "SHADER";

    std::string vsDir = "VERTEX";
    std::string fsDir = "FRAGMENT";
    std::string csDir = "COMPUTE";
    std::string gsDir = "GEOMETRY";

    for (uint32_t i = 0; i < m_Src.size(); i++) {
        if (m_Src[i] == '#') {

            bool itIsShaderDeclaration = false;
            for (; i < m_Src.size(); i++) {
                if (m_Src[i] == '\n') break;
                itIsShaderDeclaration = IsEqualDirective(startDir, i);
                if (itIsShaderDeclaration) break;
            }

            if (!itIsShaderDeclaration) continue;

            for (uint32_t j = ++i; j < m_Src.size(); j++) {
                if (m_Src[j] == '\n') break;

                if (IsEqualDirective(vsDir, j)) {
                    m_Shaders[GL_VERTEX_SHADER];
                    break;
                }

                if (IsEqualDirective(fsDir, j)) {
                    m_Shaders[GL_FRAGMENT_SHADER];
                    break;
                }

                if (IsEqualDirective(csDir, j)) {
                    m_Shaders[GL_COMPUTE_SHADER];
                    break;
                }

                if (IsEqualDirective(gsDir, j)) {
                    m_Shaders[GL_GEOMETRY_SHADER];
                    break;
                }
            }
        }
    }
}

void Shader::PreProcess() {
    PreProcessor pp;
    ParsedFile pf {m_Src, m_FileEnvironment};
    pp.PreProcess(pf);

    m_Src = pf.src;

    DefineShader();
}

uint32_t Shader::CheckShaderErrors(uint32_t shader) {
    using namespace std::string_literals;

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(shader, length, &length, message);
        Logger::LogErr("SHADER", "Failed to compile "s + m_SType + " shader!\n");
        std::cerr << message << std::endl;
        free(message);
        return -1;
    }
    return 0;
}

Shader::~Shader() {
    DeleteShader();
}

std::vector<uint32_t> Shader::GetShaders() const {
    std::vector<uint32_t> shaders;
    shaders.reserve(m_Shaders.size());
    // TODO: remade to more optimal
    for (auto& it : m_Shaders) {
        shaders.push_back(it.second);
    }

    return shaders;
}

void Shader::DeleteShader() {
    for (auto& it : m_Shaders) {
        if (it.second != 0) {
            glDeleteShader(it.second);
            it.second = 0;
        }
    }
}
