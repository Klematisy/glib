#pragma once

#include <fstream>
#include <filesystem>

#include "string"
#include "Logger/logger.h"

class PreProcessor {
public:
    PreProcessor() = default;
    void PreProcess(std::string& str, const std::string& filePath);
private:
    void ExtractInclude(std::string& fileSrc, std::string filePath);
    void DeleteRudimentarySpaces(std::string& fileSrc);
    void DeleteComments(std::string& fileSrc);
    void DeleteNextSymbolsInSequence(std::string& fileSrc, uint32_t start, char symbol);

    std::vector<std::string> m_IncludedFiles;
};