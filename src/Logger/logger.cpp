#include "logger.h"

namespace Logger {
    void Log(LogLevel level, const std::string& prefix, const std::string& message) {
        switch (level) {
            case LogLevel::INFO:    std::cout << "[INFO]["    << prefix << "]: " << message << std::endl; return;
            case LogLevel::WARNING: std::cout << "[WARNING][" << prefix << "]: " << message << std::endl; return;
            case LogLevel::ERROR:   std::cerr << "[ERROR]["   << prefix << "]: " << message << std::endl; return;
        }
    }

    void LogInf(const std::string& prefix, const std::string& message) {
        Log(LogLevel::INFO, prefix, message);
    }

    void LogWar(const std::string& prefix, const std::string& message) {
        Log(LogLevel::WARNING, prefix, message);
    }

    void LogErr(const std::string& prefix, const std::string& message) {
        Log(LogLevel::ERROR, prefix, message);
    }
}