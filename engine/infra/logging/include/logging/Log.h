#pragma once

#include <spdlog/async.h>

#include <logging/Config.h>
#include <common/Visibility.h>

namespace ly::log {

/**
 * API Responsible for lifecycle of the logger
 */
class LOG_API Log {
public:
    static std::shared_ptr<spdlog::logger> logger;

public:
    void init(LoggerConfig const& loggerConfig);
    void shutdown();

    /**
     * Log the message in the logger
     *
     * \param type The log type
     * \param log  the log message
     */
    template <typename Level, typename... Args>
    static void log(fmt::format_string<Args...> fmt, Args&&... args) {
        if constexpr (std::is_same_v<Level, Info>)
            logger->log(spdlog::level::info, fmt, std::forward<Args>(args)...);
        else if constexpr (std::is_same_v<Level, Debug>)
            logger->log(spdlog::level::debug, fmt, std::forward<Args>(args)...);
        else if constexpr (std::is_same_v<Level, Warning>)
            logger->log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
        else if constexpr (std::is_same_v<Level, Error>)
            logger->log(spdlog::level::err, fmt, std::forward<Args>(args)...);
        else if constexpr (std::is_same_v<Level, Critical>)
            logger->log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
    }
};

}  // namespace ly::log