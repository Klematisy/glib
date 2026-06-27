#pragma once

#include <string>
#include <cstdlib>
#include <vector>
#include <unordered_set>

#include "../common.h"

using CSTR = char*;

bool symEqual(const CSTR first, const CSTR second, u32 n);
int parseFile(const CSTR filepath, std::string& fileStr);

class PreProcessor {
public:
    struct FileField {
        std::string filePath;
        std::string content;
        u32 linesCount = 0;
    };

    const std::vector<FileField>& GetFields();
    int preprocessFile(const std::string& pathToCurrentFile);
private:
    std::unordered_set<std::string> m_PragmaOnceIncludeSet;
    std::vector<std::string> m_RecursionDeep;
    std::vector<FileField> m_IncludeFieldsStack;
    enum { COMPILING, ERROR } m_State = COMPILING;

    void deleteComments();
    void expandPreprocessDirectives();
};

std::vector<PreProcessor::FileField> preprocessAndDivideFile(const std::string& filePath);
