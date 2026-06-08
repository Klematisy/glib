#pragma once

#include <unordered_set>
#include <string>
#include <vector>


struct ParsedFile {
    std::string src;
    std::string path;
};

class PreProcessor {
public:
    PreProcessor() = default;
    void PreProcess(ParsedFile& pf);
private:
    std::string OpenInclude(uint32_t& temp_index, ParsedFile& pf);
    void ExtractDirectives(ParsedFile& pf);
    void DeleteRudimentarySpaces(std::string& fileSrc);
    void DeleteComments(std::string& fileSrc);
    void DeleteNextSymbolsInSequence(std::string& fileSrc, uint32_t start, char symbol);

    std::unordered_set<std::string> m_PragmaOnceFiles;
    std::vector<std::string> m_IncludedFiles;

    bool m_FatalError = false;
};
