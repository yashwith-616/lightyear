#pragma once

#include <lypch.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include "Lightyear/LightyearCore.h"

namespace ly {

enum class LogType { Trace, Info, Warn, Error, Fatal };

class LIGHTYEAR_API Log {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    template <typename... Args>
    static void LogMessage(LogType type,
                           std::shared_ptr<spdlog::logger> logger,
                           fmt::format_string<Args...> fmt,
                           Args&&... args) {
        switch (type) {
            case LogType::Trace:
                logger->trace(fmt, std::forward<Args>(args)...);
                break;
            case LogType::Info:
                logger->info(fmt, std::forward<Args>(args)...);
                break;
            case LogType::Warn:
                logger->warn(fmt, std::forward<Args>(args)...);
                break;
            case LogType::Error:
                logger->error(fmt, std::forward<Args>(args)...);
                break;
            case LogType::Fatal:
                logger->critical(fmt, std::forward<Args>(args)...);
                break;
        }
    }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
}  // namespace ly

// ===== Log Macros =====
#define LY_LOG(type, ...)      ::ly::Log::LogMessage(type, ::ly::Log::GetClientLogger(), __VA_ARGS__)
#define LY_CORE_LOG(type, ...) ::ly::Log::LogMessage(type, ::ly::Log::GetCoreLogger(), __VA_ARGS__)