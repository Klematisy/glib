#pragma once

#include <iostream>

namespace GlCore {

    namespace Logger {
        enum class LogLevel {INFO, WARNING, ERROR};

        void   Log(LogLevel level = LogLevel::INFO, const std::string& message = "");
        void Logln(LogLevel level = LogLevel::INFO, const std::string& message = "");
    }

}