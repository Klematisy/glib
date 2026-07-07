#include <filesystem>

#include "logger.h"
#include "preprocessor.h"

using CSTR = char*;

template<class T>
bool doesElementExist(std::vector<T> vec, T element) {
    return std::find(vec.begin(), vec.end(), element) != vec.cend();
}

template<class K, class T>
bool doesElementExist(std::unordered_map<K, T> map, K key) {
    return map.find(key) != map.cend();
}

template<class T>
bool doesElementExist(std::unordered_set<T> set, T val) {
    return set.find(val) != set.cend();
}

bool symEqual(const CSTR first, const CSTR second, u32 n) {
    for (u32 i = 0; i < n; i++) {
        if (first[i] != second[i]) {
            return false;
        }
    }
    return true;
}

std::vector<PreProcessor::FileField> preprocessAndDivideFile(const std::string& filePath) {
    PreProcessor pp;
    if (pp.preprocessFile(filePath) == -1) {
        LOGERR("Preprocessor error");
    }
    return pp.GetFields();
}

int parseFile(const std::string& filepath, std::string& fileStr) {
    FILE* file = fopen(filepath.c_str(), "r");

    if (!file) {
        return -1;
    }

    fseek(file, 0, SEEK_END);
    u32 size = ftell(file);
    rewind(file);

    fileStr.resize(size);
    fread(fileStr.data(), 1, size, file);

    fclose(file);

    return 0;
}

const std::vector<PreProcessor::FileField>& PreProcessor::GetFields() {
    return m_IncludeFieldsStack;
}

int PreProcessor::preprocessFile(const std::string& pathToCurrentFile)  {
    m_IncludeFieldsStack.push_back({.filePath = pathToCurrentFile});
    if (parseFile(pathToCurrentFile, m_IncludeFieldsStack.back().content) == -1) {
        LOGERR("File wasn't found '" + std::string(pathToCurrentFile) + "'");
        m_State = ERROR;
        return -1;
    }

    for (const auto& el : m_RecursionDeep) {
        if (el == pathToCurrentFile) {
            m_State = ERROR;
            LOGERR("Recursive include of files '" + std::string(pathToCurrentFile) + "'");
        }
    }

    m_RecursionDeep.push_back(pathToCurrentFile);

    deleteComments();
    expandPreprocessDirectives();

    m_RecursionDeep.pop_back();

    if (m_State == ERROR && m_RecursionDeep.size() == 0) {
        m_IncludeFieldsStack.clear();
        return -1;
    }

    return 0;
}

void PreProcessor::deleteComments() {
    auto& fileField = m_IncludeFieldsStack.back();
    bool lineComment = false;
    bool bigComment = false;

    for (u32 i = 0; i < fileField.content.size(); i++) {
        if (i + 1 < fileField.content.size()) {
            if (!bigComment && !lineComment && fileField.content[i] == '/' && fileField.content[i + 1] == '/') {
                lineComment = true;
            }

            if (!bigComment && !lineComment && fileField.content[i] == '/' && fileField.content[i + 1] == '*') {
                bigComment = true;
                fileField.content[i] = ' ';
                fileField.content[i + 1] = ' ';
            }

            if (lineComment && fileField.content[i] == '\n') {
                lineComment = false;
            }

            if (bigComment && fileField.content[i] == '*' && fileField.content[i + 1] == '/') {
                bigComment = false;
                fileField.content[i] = ' ';
                fileField.content[i + 1] = ' ';
            }
        }

        if ((bigComment || lineComment) && fileField.content[i] != '\n') {
            fileField.content[i] = ' ';
        }
    }
}

void PreProcessor::expandPreprocessDirectives() {
    u32 fileLen = m_IncludeFieldsStack.back().content.size();
    std::string mainFilePath = m_IncludeFieldsStack.back().filePath;

    const CSTR pragma = "#p_once";
    const CSTR include = "#include ";

    u32 line = 0;
    for (u32 i = 0; i < fileLen; i++) {
        auto& fileField = m_IncludeFieldsStack.back();

        if (fileField.content[i] == '\n') {
            line++;
            continue;
        }

        if ((i + 7) < fileLen && symEqual(fileField.content.data() + i, pragma, 7)) {
            if (fileField.content[i + 8] == ' ' || fileField.content[i + 8] == '\n') {
                m_PragmaOnceIncludeSet.insert(fileField.filePath);
                fileField.content.erase(i, 7);
            }
        } else if ((i + 9) < fileLen && symEqual(fileField.content.data() + i, include, 9)) {
            std::string pathToNewFile = std::filesystem::path(fileField.filePath).parent_path().string();
            if (pathToNewFile.size() != 0) pathToNewFile += "/";
            bool readerOpen = false;
            u32 directiveLen = 0;
            for (u32 j = i; j < fileLen; j++, directiveLen++) {
                if (fileField.content[j] == '"') {
                    if (readerOpen) {
                        directiveLen = j + 1 - i;
                        break;
                    }
                    readerOpen = true;
                } else if (readerOpen) {
                    pathToNewFile += fileField.content[j];
                }
            }

            memset(&fileField.content[i], 32, directiveLen);
            if (doesElementExist(m_PragmaOnceIncludeSet, pathToNewFile)) {
                continue;
            }

            FileField firstPart = m_IncludeFieldsStack.back();
            FileField thirdPart = m_IncludeFieldsStack.back();
            m_IncludeFieldsStack.pop_back();

            firstPart.content.resize(i);
            thirdPart.content.erase(0, i);

            firstPart.linesCount = line;
            m_IncludeFieldsStack.push_back(firstPart);
            line = 0;
            if (preprocessFile((const CSTR)pathToNewFile.c_str()) == -1) {
                u32 lines = 1;
                u32 column = 0;
                for (int j = i; firstPart.content[j] != '\n' && j >= 0; j--) {
                    column++;
                }

                for (auto& field : m_IncludeFieldsStack) {
                    if (mainFilePath != field.filePath) continue;
                    lines += field.linesCount;
                }
                LOGERR("ERROR: " + TO_STR(column) + ":" + TO_STR(lines) + ": Include file '" + pathToNewFile + "' wasn't found");
            }
            m_IncludeFieldsStack.push_back(thirdPart);
            i = 0;
        }
        fileLen = m_IncludeFieldsStack.back().content.size();
    }
    m_IncludeFieldsStack.back().linesCount = line;
}
