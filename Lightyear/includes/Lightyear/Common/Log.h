#pragma once

#include "Lightyear/Common/Base.h"
#include "Lightyear/Common/Macros.h"
#include "Lightyear/Common/Math.h"

#ifdef SPDLOG_FWRITE_UNLOCKED
    #undef SPDLOG_FWRITE_UNLOCKED
#endif

LY_DISABLE_WARNINGS_PUSH
#include <spdlog/fmt/ostr.h>

#include "spdlog/spdlog.h"
LY_DISABLE_WARNINGS_POP

namespace ly {

enum class LogType { Trace, Info, Warn, Error, Fatal };

class LIGHTYEAR_API Log {
public:
    static void init();
    static void shutdown();

    static ref<spdlog::logger>& getCoreLogger() { return m_sCoreLogger; }
    static ref<spdlog::logger>& getClientLogger() { return m_sClientLogger; }

    // Template specialization for compile-time format strings (fmt::format_string)
    template <typename... Args>
    static void logMessage(LogType type, ref<spdlog::logger> logger, fmt::format_string<Args...> fmt, Args&&... args) {
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
    static ref<spdlog::logger> m_sCoreLogger;
    static ref<spdlog::logger> m_sClientLogger;
};

template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
OStream& operator<<(OStream& os, glm::vec<L, T, Q> const& vector) {
    return os << glm::to_string(vector);
}

template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
OStream& operator<<(OStream& os, glm::mat<C, R, T, Q> const& matrix) {
    return os << glm::to_string(matrix);
}

template <typename OStream, typename T, glm::qualifier Q>
OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion) {
    return os << glm::to_string(quaternion);
}

}  // namespace ly

// ===== Log Macros =====
#ifdef LY_DEBUG
    #define LY_LOG(type, ...) ::ly::Log::logMessage(type, ::ly::Log::getClientLogger(), __VA_ARGS__)
    #define LY_CORE_LOG(type, ...) ::ly::Log::logMessage(type, ::ly::Log::getCoreLogger(), __VA_ARGS__)
#else
    #define LY_LOG(type, ...)
    #define LY_CORE_LOG(type, ...)
#endif