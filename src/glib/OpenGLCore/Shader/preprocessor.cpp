#include "preprocessor.h"

static void ParseFile(const char* filePath, std::string& src) {
    std::ifstream file(filePath);
    using namespace std::string_literals;
    std::string ch;

    if (!file.is_open()) {
        Logger::LogErr("SHADER", "File '"s + filePath + "' isn't open!");
    }

    while (getline(file, ch)) {
        src.append(ch + '\n');
    }
}


void PreProcessor::DeleteNextSymbolsInSequence(std::string &fileSrc, uint32_t start, char symbol) {
    for (uint32_t j = start; j < fileSrc.size(); j++) {
        if (fileSrc[j] == '\n') break;
        if (fileSrc[j] != symbol) {
            fileSrc.erase(start, j - start);
            break;
        }
    }
}



void PreProcessor::DeleteRudimentarySpaces(std::string& fileSrc) {
    for (uint32_t i = 0; i < fileSrc.size(); i++) {
        if (i == 0 && fileSrc[i] == ' ') {
            DeleteNextSymbolsInSequence(fileSrc, 0, ' ');
        }

        if (i > 0) {
            if (fileSrc[i - 1] == '\n' && fileSrc[i] == ' ') {
                DeleteNextSymbolsInSequence(fileSrc, i, ' ');
            }
        }

        if (fileSrc[i] == '#') {
            i++;
            for (uint32_t j = i; j < fileSrc.size(); j++) {
                if (fileSrc[j] != ' ') {
                    fileSrc.erase(i, j - i);
                    break;
                }
            }

            i++;
            uint32_t spaceCount = 0;
            uint32_t hookPoint = i;
            for (uint32_t j = i; j < fileSrc.size(); j++) {
                if (fileSrc[j] == ' ') {
                    if (spaceCount == 0) hookPoint = j;
                    spaceCount++;
                }
                if (fileSrc[j] != ' ') {
                    if (spaceCount > 1) {
                        fileSrc.erase(hookPoint, spaceCount - 1);
                    }
                    spaceCount = 0;
                    hookPoint = j;
                }
            }
        }
    }
}

std::string PreProcessor::OpenInclude(uint32_t& temp_index, ParsedFile& pf) {;

    std::string includeDirective = "include";
    ParsedFile resultPf;

    temp_index += includeDirective.size() + 2;
    std::string fileName;

    while (temp_index < pf.src.size() && pf.src[temp_index] != '<' && pf.src[temp_index] != '>' && pf.src[temp_index] != '"') {
        fileName += pf.src[temp_index++];
    }

    temp_index++;

    std::string pathToDir = std::filesystem::path(pf.path).parent_path().string() + '/';
    std::string fullPath = std::filesystem::weakly_canonical(std::filesystem::path(pathToDir + fileName)).string();

    if (m_PragmaOnceFiles.find(fullPath) != m_PragmaOnceFiles.cend()) {
        bool addFile = true;
        for (const auto& it : m_IncludedFiles) {
            if (it == fullPath) {
                return resultPf.src;
            }
        }
    }

    ParseFile((fullPath).c_str(), resultPf.src);
    ParsedFile pf2 {resultPf.src, fullPath};
    PreProcess(pf2);

    return resultPf.src;
}

static bool IntervalIsEqual(const std::string& fileSrc, uint32_t index, const std::string& directive) {
    bool equal = true;
    for (uint32_t j = 0; j < directive.size() && index + j < fileSrc.size(); j++) {
        equal = true;
        if (directive[j] != fileSrc[index + j]) {
            equal = false;
            break;
        }
    }
    return equal;
}

void PreProcessor::ExtractDirectives(ParsedFile& pf) {
    using namespace std::string_literals;
    std::string includeDirective = "include";
    std::string pragmaOnceDirective = "pragma once";

    for (uint32_t i = 0; i < pf.src.size(); i++) {
        if (pf.src[i] == '#') {
            uint32_t temp_index = i + 1;

            bool itIsPragmaOnce = IntervalIsEqual(pf.src, temp_index, pragmaOnceDirective);
            if (itIsPragmaOnce) {
                m_PragmaOnceFiles.insert(pf.path);

                temp_index += pragmaOnceDirective.size();
                pf.src.erase(i, temp_index - i);
                continue;
            }

            bool itIsIncludeDirective = IntervalIsEqual(pf.src, temp_index, includeDirective);
            if (itIsIncludeDirective) {
                std::string includeFile = OpenInclude(temp_index, pf);

                pf.src.erase(i, temp_index - i);
                pf.src.insert(i, includeFile);
                continue;
            }
        }
    }
}

void PreProcessor::DeleteComments(std::string& fileSrc) {
    using namespace std::string_literals;

    for (uint32_t i = 0; i < fileSrc.size() - 1; i++) {
        if (fileSrc[i] == '/' && fileSrc[i + 1] == '/') {
            uint32_t commentSize = 0;
            for (uint32_t j = i; j < fileSrc.size(); j++) {
                if (fileSrc[j] == '\n') break;
                commentSize++;
            }
            fileSrc.erase(i, commentSize);
        }

        if (fileSrc[i] == '/' && fileSrc[i + 1] == '*') {
            uint32_t commentSize = 0;
            for (uint32_t j = i; j < fileSrc.size() - 1; j++) {
                if (fileSrc[j] == '*' && fileSrc[j + 1] == '/') break;
                if (j == fileSrc.size() - 2) {
                    Logger::LogErr("GLSL", "/* - isn't end!");
                    commentSize = 0;
                    break;
                }
                commentSize++;
            }
            fileSrc.erase(i, commentSize + 2);
        }
    }
}

void PreProcessor::PreProcess(ParsedFile& pf) {
    pf.path = std::filesystem::absolute(pf.path).string();
    m_IncludedFiles.push_back(pf.path);

    DeleteComments(pf.src);
    DeleteRudimentarySpaces(pf.src);
    ExtractDirectives(pf);
}