#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <cstdint>

namespace Lumina {

enum class LogLevel : uint8_t {
    None = 0,
    Error = 1,
    Warning = 2,
    Info = 3,
    Debug = 4
};

class Logger {
public:
    static void Initialize(bool enabled = false, LogLevel level = LogLevel::None);
    static void Shutdown();
    
    static void SetEnabled(bool enabled);
    static void SetLevel(LogLevel level);
    
    template<typename... Args>
    static void Error(const std::string& fmt, Args&&... args) {
        if (s_enabled && s_level >= LogLevel::Error) {
            Log(LogLevel::Error, Format(fmt, std::forward<Args>(args)...));
        }
    }

    template<typename... Args>
    static void Warning(const std::string& fmt, Args&&... args) {
        if (s_enabled && s_level >= LogLevel::Warning) {
            Log(LogLevel::Warning, Format(fmt, std::forward<Args>(args)...));
        }
    }

    template<typename... Args>
    static void Info(const std::string& fmt, Args&&... args) {
        if (s_enabled && s_level >= LogLevel::Info) {
            Log(LogLevel::Info, Format(fmt, std::forward<Args>(args)...));
        }
    }

    template<typename... Args>
    static void Debug(const std::string& fmt, Args&&... args) {
        if (s_enabled && s_level >= LogLevel::Debug) {
            Log(LogLevel::Debug, Format(fmt, std::forward<Args>(args)...));
        }
    }
    
    static bool IsEnabled() { return s_enabled; }
    static LogLevel GetLevel() { return s_level; }

private:
    static void Log(LogLevel level, const std::string& message);
    static const char* LevelToString(LogLevel level);
    
    template<typename... Args>
    static std::string Format(const std::string& fmt, Args&&... args) {
        if constexpr (sizeof...(args) == 0) {
            return fmt;
        } else {
            char buffer[1024];
            std::snprintf(buffer, sizeof(buffer), fmt.c_str(), std::forward<Args>(args)...);
            return std::string(buffer);
        }
    }
    
    static bool s_enabled;
    static LogLevel s_level;
    static std::unique_ptr<std::ofstream> s_logFile;
};

// Logging macros (evaluate to nothing if disabled)
#ifdef LUMINA_LOGGING_ENABLED
    #define LUMINA_LOG_ERROR(fmt, ...) Lumina::Logger::Error(fmt, ##__VA_ARGS__)
    #define LUMINA_LOG_WARNING(fmt, ...) Lumina::Logger::Warning(fmt, ##__VA_ARGS__)
    #define LUMINA_LOG_INFO(fmt, ...) Lumina::Logger::Info(fmt, ##__VA_ARGS__)
    #define LUMINA_LOG_DEBUG(fmt, ...) Lumina::Logger::Debug(fmt, ##__VA_ARGS__)
#else
    #define LUMINA_LOG_ERROR(fmt, ...) ((void)0)
    #define LUMINA_LOG_WARNING(fmt, ...) ((void)0)
    #define LUMINA_LOG_INFO(fmt, ...) ((void)0)
    #define LUMINA_LOG_DEBUG(fmt, ...) ((void)0)
#endif

} // namespace Lumina
