#include <fstream>
#include <filesystem>
#include <cassert>

#include "Logger/logger.h"
#include "shader.h"
#include "preprocessor.h"

using namespace RendererCore;
using namespace GAPI;

static void ParseFile(const char* filePath, std::string& src) {
    std::ifstream file(filePath);
    using namespace std::string_literals;
    std::string ch;

    if (!file.is_open()) {
        Logger::LogErr("SHADER", "File '"s + filePath + "' isn't open!");
        assert(1);
    }

    while (getline(file, ch)) {
        src.append(ch + '\n');
    }
}


static auto& gapi = GraphicsAPIImpl::Get();

void Shader::SetShaderSourceFile(const char* filePath) {
    ParseFile(filePath, m_Src);
    m_FileEnvironment = std::filesystem::path(filePath).string();
}

std::string Shader::GetDefineShader(GAPI::SHADER_TYPE shader_type) {
    switch (shader_type) {
        case GAPI::SHADER_TYPE::VERTEX:   m_SType = "VERTEX";   break;
        case GAPI::SHADER_TYPE::FRAGMENT: m_SType = "FRAGMENT"; break;
        case GAPI::SHADER_TYPE::COMPUTE:  m_SType = "COMPUTE";  break;
        case GAPI::SHADER_TYPE::GEOMETRY: m_SType = "GEOMETRY"; break;
        default:                          m_SType = "UNKNOWN";
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

        const char* specified_shader[] = {
                "#version 410 core\n",  // TODO: flexible version
                define_shader.c_str(),
                m_Src.c_str()
        };

        m_Id = gapi.CreateShader(it.first);
        gapi.ShaderSource(m_Id, 3, specified_shader, nullptr);
        gapi.CompileShader(m_Id);

        if (CheckShaderErrors(m_Id) == -1) return;

        m_Shaders[it.first] = m_Id;
    }
}

bool Shader::IsEqualDirective(const std::string& directive, uint32_t index) {
    for (uint32_t k = 0; k < directive.size(); k++) {
        if (m_Src[index + k] != directive[k])
            return false;
    }
    return true;
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
                    m_Shaders[GAPI::SHADER_TYPE::VERTEX] = 0;
                    break;
                }

                if (IsEqualDirective(fsDir, j)) {
                    m_Shaders[GAPI::SHADER_TYPE::FRAGMENT] = 0;
                    break;
                }

                if (IsEqualDirective(csDir, j)) {
                    m_Shaders[GAPI::SHADER_TYPE::COMPUTE] = 0;
                    break;
                }

                if (IsEqualDirective(gsDir, j)) {
                    m_Shaders[GAPI::SHADER_TYPE::GEOMETRY] = 0;
                    break;
                }
            }
        }
    }
}

static int a = 0;
void Shader::PreProcess() {
    PreProcessor pp;
    ParsedFile pf {m_Src, m_FileEnvironment};
    pp.PreProcess(pf);

    m_Src = pf.src;

#ifdef __GLIB_GEN_PREPROCESSED_SHADER_SRC__
    using namespace std::string_literals;
    std::ofstream file("shader_cache/" + std::filesystem::path(m_FileEnvironment).filename().string());
    file << m_Src;
    file.close();
#endif

    DefineShader();
}

uint32_t Shader::CheckShaderErrors(uint32_t shader) {
    using namespace std::string_literals;

    int result;
    gapi.GetShaderiv(shader, GAPI::SHADER_COMPILE::STATUS, &result);
    if (!result) {
        int length;
        gapi.GetShaderiv(shader, GAPI::SHADER_COMPILE::INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        gapi.GetShaderInfoLog(shader, length, &length, message);
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
        gapi.DeleteShader(&it.second);
    }
}
