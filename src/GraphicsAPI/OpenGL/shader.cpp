#include <filesystem>
#include <fstream>
#include <bit>

#include "graphics_api_opengl.h"

#include "type_casting.h"
#include "Logger/logger.h"

#include "preprocessor.h"

std::string GAPI::parseFile(const std::string& filePath) {
    std::ifstream file(filePath);
    using namespace std::string_literals;
    std::string ch;

    if (!file.is_open()) {
        LOGERR("PARSER: File '"s + filePath + "' isn't open!");
        assert(1);
    }

    std::string result;
    while (getline(file, ch)) {
        result.append(ch + '\n');
    }

    return result;
}

std::string GAPI::getShaderTypeInStr(GAPI::SHADER_TYPE type) {
    switch (type) {
        case GAPI::SHADER_TYPE::VERTEX:   return "VERTEX";   break;
        case GAPI::SHADER_TYPE::FRAGMENT: return "FRAGMENT"; break;
        case GAPI::SHADER_TYPE::COMPUTE:  return "COMPUTE";  break;
        case GAPI::SHADER_TYPE::GEOMETRY: return "GEOMETRY"; break;
    }
    return "";
}

ShaderOpenGL::ShaderOpenGL(std::string filePath, GAPI::SHADER_TYPE shaderType)
    : Shader()
{
    m_Id = glCreateShader(GL::SHADER_TYPE[std::countr_zero(u32(shaderType))]);
    m_FilePath = filePath;
    m_ShaderType = shaderType;
}

ShaderOpenGL::ShaderOpenGL(ShaderOpenGL&& other) noexcept {
    m_ShaderType = other.m_ShaderType;
    m_FilePath = other.m_FilePath;
    m_Id = other.m_Id;

    other.m_FilePath = "";
    other.m_Id = 0;
}

ShaderOpenGL& ShaderOpenGL::operator=(ShaderOpenGL&& other) noexcept {
    m_ShaderType = other.m_ShaderType;
    m_FilePath = other.m_FilePath;
    m_Id = other.m_Id;

    other.m_FilePath = "";
    other.m_Id = 0;
    return *this;
}

ShaderOpenGL::~ShaderOpenGL() {
    if (m_Id != 0)
        glDeleteShader(m_Id);
}

void ShaderCompilerOpenGL::convertErrorLogToReadable(const std::vector<PreProcessor::FileField>& fields, i32 i) {
    m_ErrorLog.erase(i, 7);

    std::string columnStr;
    std::string lineStr;
    bool isColumnReading = true;
    for (u32 j = i; j < m_ErrorLog.size(); j++) {
        if (m_ErrorLog[j] == ':') {
            if (!isColumnReading) {
                break;
            }
            isColumnReading = !isColumnReading;
            continue;
        }

        if (isColumnReading) {
            columnStr += m_ErrorLog[j];
        } else {
            lineStr += m_ErrorLog[j];
        }
    }

    u32 column = std::stoi(columnStr);
    u32 globalLine = std::stoi(lineStr);

    std::string errFileName;
    u32 localLine = 2;
    u32 localColumn = 0;

    const PreProcessor::FileField* fieldPtr = nullptr;
    for (const auto& field : fields) {
        localLine += field.linesCount;
        if (localLine >= globalLine) {
            fieldPtr = &field;
            errFileName = field.filePath;
            break;
        }
    }

    localLine = 0;
    for (const auto& field : fields) {
        if (&field == fieldPtr) {
            break;
        }
        if (field.filePath == errFileName) {
            continue;
        }

        localLine += field.linesCount;
    }


    std::string result_err = std::filesystem::absolute(errFileName).string() + ":" + TO_STR(globalLine - localLine) + ":" + TO_STR(localColumn);

    m_ErrorLog.erase(i, lineStr.size() + columnStr.size() + 1);
    m_ErrorLog.insert(i, result_err);
}

void ShaderCompilerOpenGL::Compile(Shader* shader) {
    ShaderOpenGL* shaderInstance = (ShaderOpenGL*)shader;

    auto shaderFields = preprocessAndDivideFile(shaderInstance->m_FilePath);
    for (i32 i = precompiledOptions.size() - 1; 0 <= i; i--) {
        shaderFields.insert(shaderFields.begin(),
            (PreProcessor::FileField) {
                .filePath = "PREPROCESS_DEFINITION_" + TO_STR(i),
                .content = precompiledOptions[i],
                .linesCount = 1
            }
        );
    }

    std::string resultSrc;
    for (const auto& field : shaderFields) {
        resultSrc += field.content;
    }

    /*  LOG EXAMPLE
     * GLSL LOG:
     * ERROR: 0:14: 'uniform' : syntax error: syntax error
     */

    const char* resultCSTR = resultSrc.c_str();
    glShaderSource(shaderInstance->m_Id, 1, &resultCSTR, nullptr);
    glCompileShader(shaderInstance->m_Id);
    if (CheckShaderErrors(shaderInstance) == -1) {
        std::string columnStr;
        std::string lineStr;

        u32 errCoordLen = 0;
        u32 doublePoint = 0;
        for (u32 i = 0; i < m_ErrorLog.size(); i++, errCoordLen++) {
            if (symEqual(&m_ErrorLog[i], "ERROR: ", 7)) {
                convertErrorLogToReadable(shaderFields, i);
            }
        }

        LOGERR("GLSL LOG:\n" + m_ErrorLog);

        m_ErrorLog.clear();
    }
}

i32 ShaderCompilerOpenGL::CheckShaderErrors(Shader* shader) {
    using namespace std::string_literals;
    ShaderOpenGL* shaderInstance = (ShaderOpenGL*)shader;
    int result;
    glGetShaderiv(shaderInstance->m_Id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length;
        glGetShaderiv(shaderInstance->m_Id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length);
        glGetShaderInfoLog(shaderInstance->m_Id, length, &length, message);
        LOGERR("SHADER : Failed to compile "s + getShaderTypeInStr(shaderInstance->m_ShaderType) + " shader!");
        m_ErrorLog = message;
        free(message);
        return -1;
    }
    return 0;
}


ShaderProgramOpenGL& ShaderProgramOpenGL::operator=(ShaderProgramOpenGL&& other) {
    m_ShaderProgram = other.m_ShaderProgram;
    m_AttachedShaders = std::move(other.m_AttachedShaders);
    m_UniformLocations = std::move(other.m_UniformLocations);

    other.m_ShaderProgram = 0;

    return *this;
}

ShaderProgramOpenGL& ShaderProgramOpenGL::operator=(const ShaderProgramOpenGL& other) {
    m_ShaderProgram = other.m_ShaderProgram;
    m_AttachedShaders = other.m_AttachedShaders;
    m_UniformLocations = other.m_UniformLocations;

    return *this;
}

ShaderProgramOpenGL::ShaderProgramOpenGL() {
    m_ShaderProgram = glCreateProgram();
}

ShaderProgramOpenGL::~ShaderProgramOpenGL() {
    UnBind();
    glDeleteProgram(m_ShaderProgram);
}

void ShaderProgramOpenGL::LinkProgram() {
    if (m_ShaderProgram != 0) {
        UnBind();
        glDeleteProgram(m_ShaderProgram);
        m_ShaderProgram = glCreateProgram();
    }

    for (auto& shader : m_AttachedShaders) {
        if (shader) {
            ShaderOpenGL* shaderInstance = (ShaderOpenGL*)shader;
            glAttachShader(m_ShaderProgram, shaderInstance->m_Id);
        } else {
            LOGERR("Shader Linker: shader is dead!");
        }
    }

    glLinkProgram(m_ShaderProgram);
    CheckLinkingErrors();
}

i32 ShaderProgramOpenGL::CheckLinkingErrors() const {
    int result;
    glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &result);
    if (!result) {
        int length;
        glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)malloc(length * sizeof(char));
        glGetProgramInfoLog(m_ShaderProgram, length, nullptr, message);
        LOGERR("SHADER PROGRAM: Failed to link program!");
        free(message);
    }
}

void ShaderProgramOpenGL::AttachShader(const Shader* shader) {
    m_AttachedShaders.push_back(shader);
}

void ShaderProgramOpenGL::Bind() const {
    if (m_ShaderProgram == 0)
        LOGWARN("SHADER PROGRAM: This sp doesn't exist!"); // TODO: improve this log in a future

    assert(m_ShaderProgram != 0);

    glUseProgram(m_ShaderProgram);
}

void ShaderProgramOpenGL::UnBind() const {
    glUseProgram(0);
}

void ShaderProgramOpenGL::ClearShaders() {
    m_AttachedShaders.clear();
}




int ShaderProgramOpenGL::GetUniformLocation(const std::string& name) const {
    if (m_UniformLocations.find(name) == m_UniformLocations.end())
        m_UniformLocations[name] = glGetUniformLocation(m_ShaderProgram, name.c_str());

    return m_UniformLocations[name];
}

void ShaderProgramOpenGL::SetInt(const std::string& name, int value) const {
    Bind();
    glUniform1i(GetUniformLocation(name), value);
    UnBind();
}

void ShaderProgramOpenGL::SetFloat(const std::string& name, float value) const {
    Bind();
    glUniform1f(GetUniformLocation(name), value);
    UnBind();
}

void ShaderProgramOpenGL::SetFloat2(const std::string& name, const glm::vec2& value) const {
    Bind();
    glUniform2f(GetUniformLocation(name), value.x, value.y);
    UnBind();
}

void ShaderProgramOpenGL::SetIntArray(const std::string& name, uint32_t count, const int* value) const {
    Bind();
    glUniform1iv(GetUniformLocation(name), count, value);
    UnBind();
}

void ShaderProgramOpenGL::SetMatrixFloat4(const std::string& name, const float* value_ptr) const {
    Bind();
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value_ptr);
    UnBind();
}
