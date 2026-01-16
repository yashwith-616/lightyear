#pragma once

#include <filesystem>
#include <optional>
#include <string>

#pragma region Macros Visbility

#if defined(LOGGING_STATIC)
    #define LOG_API
#elif defined(LOGGING_BUILD_DLL)
    #define LOG_API LY_DLL_EXPORT
#else
    #define LOG_API LY_DLL_IMPORT
#endif

#pragma endregion

namespace ly::log {

struct MemorySinkConfig {
    // Buffer size of the ring buffer
    size_t bufferSize;

    // Output pattern of the log. Usage: [%H:%M:%S.%e] [%^%l%$] %v
    std::string logFormatPattern;
};

struct FileSinkConfig {
    // example: lightyear-%Y-%m-%d.log;
    std::filesystem::path fileNamePattern;

    // When number of files exceeded than max files, oldest files are deleted.
    int maxFiles{ 10 };
};

/**
 * The following struct will be utilized to initialize logger
 */
struct LoggerConfig {
    std::string loggerName;
    size_t asyncQueueSize;
    int loggerThreadCount;

    std::optional<MemorySinkConfig> memSinkInfo;
    std::optional<FileSinkConfig> fileSinkInfo;
    bool isConsoleLoggingEnabled{ true };
};

//-----------------------------------------------
// Tag Types for templated log
//-----------------------------------------------
struct Info {};
struct Warning {};
struct Debug {};
struct Error {};
struct Critical {};

}  // namespace ly::log