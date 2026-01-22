#include <logging/Log.h>

#include <memory>

int main()
{
    ly::log::FileSinkConfig fileConfig{.fileNamePattern = "lightyear-%Y-%m-%d.log", .maxFiles = 7};
    ly::log::MemorySinkConfig memoryConfig{.bufferSize = 4096, .logFormatPattern = "[%H:%M:%S.%e] [%^%l%$] %v"};
    ly::log::LoggerConfig loggerConfig{
        .loggerName = "example_basic",
        .asyncQueueSize = 8192,
        .loggerThreadCount = 1,
        .memSinkInfo = memoryConfig,
        .fileSinkInfo = fileConfig,
        .isConsoleLoggingEnabled = true};

    namespace logger = ly::log;

    // Initialize the log system at the root of the application
    auto logSystem = std::make_unique<logger::Log>();
    logSystem->init(loggerConfig);

    // Wrap the following construct in any macro or simple wrapper. Since DLL are initialized after the application has
    // started the following code works.
    logger::Log::log<logger::Info>("info");
    logger::Log::log<logger::Debug>("debug");
    logger::Log::log<logger::Warning>("warning");
    logger::Log::log<logger::Error>("error");
    logger::Log::log<logger::Critical>("critical");

    // Called in the main application
    logSystem->shutdown();
}
