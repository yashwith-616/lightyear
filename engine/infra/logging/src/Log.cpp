#include <../include/logging/Log.h>

#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

namespace ly::log {

std::shared_ptr<spdlog::logger> Log::logger = nullptr;

void Log::init(LoggerConfig const& loggerConfig) {
    spdlog::init_thread_pool(loggerConfig.asyncQueueSize, loggerConfig.loggerThreadCount);

    std::vector<spdlog::sink_ptr> sinks{};

    // configure memory sink
    if (loggerConfig.memSinkInfo.has_value()) {
        auto const& memSinkInfo = *loggerConfig.memSinkInfo;
        auto ringbufferSink     = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(memSinkInfo.bufferSize);
        ringbufferSink->set_pattern(memSinkInfo.logFormatPattern);
        sinks.push_back(ringbufferSink);
    }

    // configure file sink
    if (loggerConfig.fileSinkInfo.has_value()) {
        auto const& fileSinkInfo = *loggerConfig.fileSinkInfo;
        assert(fileSinkInfo.maxFiles > 0 && "FileSink max files must be greater than 0");

        auto dailyFileSync = std::make_shared<spdlog::sinks::daily_file_format_sink_mt>(
            fileSinkInfo.fileNamePattern.string(), 0, 0, false, 100);
        sinks.push_back(dailyFileSync);
    }

    // configure console appender
    if (loggerConfig.isConsoleLoggingEnabled) {
        auto console     = spdlog::stderr_logger_mt("console");
        auto consoleSink = std::make_shared<spdlog::sinks::stderr_sink_mt>();
        sinks.push_back(consoleSink);
    }

    logger = std::make_shared<spdlog::async_logger>(loggerConfig.loggerName,
                                                    sinks.begin(),
                                                    sinks.end(),
                                                    spdlog::thread_pool(),
                                                    spdlog::async_overflow_policy::overrun_oldest);

    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::err);
    logger->set_error_handler([](std::string const& msg) { fprintf(stderr, "spdlog error: %s\n", msg.c_str()); });

    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);
}

void Log::shutdown() {
    if (!logger) {
        return;
    }

    logger->flush();
    spdlog::drop(logger->name());
    logger.reset();
    spdlog::shutdown();
}

}  // namespace ly::log
