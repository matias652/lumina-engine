#include "LuminaEngine/utils/Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Lumina {

bool Logger::s_enabled = false;
LogLevel Logger::s_level = LogLevel::None;
std::unique_ptr<std::ofstream> Logger::s_logFile = nullptr;

void Logger::Initialize(bool enabled, LogLevel level) {
    s_enabled = enabled;
    s_level = level;
}

void Logger::Shutdown() {
    if (s_logFile && s_logFile->is_open()) {
        s_logFile->close();
    }
    s_logFile.reset();
}

void Logger::SetEnabled(bool enabled) {
    s_enabled = enabled;
}

void Logger::SetLevel(LogLevel level) {
    s_level = level;
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (!s_enabled) {
        return;
    }
    
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    // Use thread-safe variants: localtime_r (POSIX) / localtime_s (MSVC)
#if defined(_WIN32)
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif

    const char* color = "";
    const char* reset = "\033[0m";
    switch (level) {
        case LogLevel::Error:   color = "\033[1;31m"; break; // Red
        case LogLevel::Warning: color = "\033[1;33m"; break; // Yellow
        case LogLevel::Info:    color = "\033[1;32m"; break; // Green
        case LogLevel::Debug:   color = "\033[1;36m"; break; // Cyan
        default:                break;
    }

    std::stringstream ss;
    ss << "[" << std::put_time(&tm, "%H:%M:%S") << "] "
       << "[" << LevelToString(level) << "] " 
       << message;

    if (level == LogLevel::Error) {
        std::cerr << color << ss.str() << reset << std::endl;
    } else {
        std::cout << color << ss.str() << reset << std::endl;
    }

    if (s_logFile && s_logFile->is_open()) {
        *s_logFile << ss.str() << std::endl;
    }
}

const char* Logger::LevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Error:   return "ERROR";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Info:    return "INFO";
        case LogLevel::Debug:   return "DEBUG";
        default:                return "UNKNOWN";
    }
}

} // namespace Lumina
