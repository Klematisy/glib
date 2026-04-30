#include <iostream>
#include <chrono>
#include <format>

#include "logger.h"

namespace Logger {
    void LogInf(const char* func, const std::string& message) {
        auto now = std::chrono::system_clock::now();
        std::string time = std::format("{:%H:%M:%S}", now);
        std::cout << "[" << std::format("{:%H:%M:%S}", now) << "]"
                  << "[INFO]"
                  << "[" << func << "]: " << message << std::endl;
    }

    void LogWar(const char* func, const std::string& message) {
        auto now = std::chrono::system_clock::now();
        std::string time = std::format("{:%H:%M:%S}", now);
        std::cout << "[" << std::format("{:%H:%M:%S}", now) << "]"
                  << "[WARNING]"
                  << "[" << func << "]: " << message << std::endl;
    }

    void LogErr(const char* func, const std::string& message) {
        auto now = std::chrono::system_clock::now();
        std::string time = std::format("{:%H:%M:%S}", now);
        std::cerr << "[" << std::format("{:%H:%M:%S}", now) << "]"
                  << "[ERROR]"
                  << "[" << func << "]: " << message << std::endl;
    }
}