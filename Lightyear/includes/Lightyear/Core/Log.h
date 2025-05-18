#pragma once

#include <lypch.h>
#include "Base.h"
#include "Macros.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace ly {

enum class LogType { Trace, Info, Warn, Error, Fatal };

class LIGHTYEAR_API Log {
public:
    static void Init();
    static void Shutdown();

    inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    // Template specialization for compile-time format strings (fmt::format_string)
    template <typename... Args>
    static void LogMessage(LogType type,
                           Ref<spdlog::logger> logger,
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
    static Ref<spdlog::logger> s_CoreLogger;
    static Ref<spdlog::logger> s_ClientLogger;
};

template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector) {
    return os << glm::to_string(vector);
}

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix) {
    return os << glm::to_string(matrix);
}

template <typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion) {
    return os << glm::to_string(quaternion);
}

}  // namespace ly

// ===== Log Macros =====
#define LY_LOG(type, ...) ::ly::Log::LogMessage(type, ::ly::Log::GetClientLogger(), __VA_ARGS__)
#define LY_CORE_LOG(type, ...) ::ly::Log::LogMessage(type, ::ly::Log::GetCoreLogger(), __VA_ARGS__)