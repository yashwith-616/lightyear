#pragma once

#include <logging/Log.h>
#include <memory>

#define LOG(x, y) ly::log::Log::log<x>(y)

namespace logger = ly::log;

class LogScope
{
public:
    LogScope()
    {
        logger::LoggerConfig config{
            .loggerName = "example_basic",
            .asyncQueueSize = 8192,
            .loggerThreadCount = 1,
            .isConsoleLoggingEnabled = true};

        m_log = std::make_unique<logger::Log>();
        m_log->init(config);
    }

    ~LogScope()
    {
        if (m_log)
        {
            m_log->shutdown();
        }
    }

    logger::Log& get() { return *m_log; }

private:
    std::unique_ptr<logger::Log> m_log;
};
