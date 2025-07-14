#pragma once

#include <iostream>

namespace Logger {
    enum class LogLevel {INFO, WARNING, ERROR};

    void   Log(LogLevel level = LogLevel::INFO, const std::string& message = "");

    void LogInf(const std::string& message);
    void LogWar(const std::string& message);
    void LogErr(const std::string& message);
}