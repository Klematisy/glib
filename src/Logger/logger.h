#pragma once

#define LOGINF(msg)  Logger::LogInf(__func__, msg)
#define LOGWARN(msg) Logger::LogInf(__func__, msg)
#define LOGERR(msg)  Logger::LogInf(__func__, msg)

#include <string>

#define TO_STR(msg) std::to_string(msg)

namespace Logger {
    void LogInf(const char* func, const std::string& message);
    void LogWar(const char* func, const std::string& message);
    void LogErr(const char* func, const std::string& message);
}