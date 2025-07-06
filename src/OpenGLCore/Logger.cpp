#include "Logger.h"

namespace GlCore {

    namespace Logger {
        void Log(LogLevel level, const std::string& message) {
            switch (level) {
                case LogLevel::INFO:    std::cout << "INFO: "    << message; return;
                case LogLevel::WARNING: std::cout << "WARNING: " << message; return;
                case LogLevel::ERROR:   std::cerr << "ERROR: "   << message; return;
            }
        }

        void Logln(LogLevel level, const std::string& message) {
            Log(level, message);
            std::cout << '\n';
        }
    }

}