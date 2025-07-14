#include "Logger.h"

namespace Logger {
    void Log(LogLevel level, const std::string& message) {
        switch (level) {
            case LogLevel::INFO:    std::cout << "INFO: "    << message << std::endl; return;
            case LogLevel::WARNING: std::cout << "WARNING: " << message << std::endl; return;
            case LogLevel::ERROR:   std::cerr << "ERROR: "   << message << std::endl; return;
        }
    }

    void LogInf(const std::string& message) {
        Log(LogLevel::INFO, message);
    }

    void LogWar(const std::string& message) {
        Log(LogLevel::WARNING, message);
    }

    void LogErr(const std::string& message) {
        Log(LogLevel::ERROR, message);
    }
}