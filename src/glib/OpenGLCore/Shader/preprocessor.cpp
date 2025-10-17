#include "preprocessor.h"

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

        if (fileSrc[i - 1] == '\n' && fileSrc[i] == ' ') {
            DeleteNextSymbolsInSequence(fileSrc, i, ' ');
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

void PreProcessor::ExtractInclude(std::string& fileSrc, std::string filePath) {
    // TODO: pragma once in glsl

    std::string includeDirective = "include";

    for (uint32_t i = 0; i < fileSrc.size(); i++) {
        if (fileSrc[i] == '#') {
            uint32_t temp_index = i + 1;

            bool itIsIncludeDirective = false;
            for (uint32_t j = 0; j < includeDirective.size() && temp_index + j < fileSrc.size(); j++) {
                itIsIncludeDirective = true;
                if (includeDirective[j] != fileSrc[temp_index + j]) {
                    itIsIncludeDirective = false;
                    break;
                }
            }

            if (itIsIncludeDirective) {
                temp_index += includeDirective.size() + 2;
                std::string fileName;

                while (temp_index < fileSrc.size() && fileSrc[temp_index] != '<' && fileSrc[temp_index] != '>' && fileSrc[temp_index] != '"') {
                    fileName += fileSrc[temp_index++];
                }

                temp_index++;
                std::string includeFile;
                ParseFile((filePath + fileName).c_str(), includeFile);
                PreProcess(includeFile, filePath + fileName);
                fileSrc.erase(i, temp_index - i);
                fileSrc.insert(i, includeFile);
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

void PreProcessor::PreProcess(std::string &str, const std::string& filePath) {
    DeleteComments(str);
    DeleteRudimentarySpaces(str);
    ExtractInclude(str, filePath);
}