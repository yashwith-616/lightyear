#include "Lightyear/Common/Log.h"

#include "Lightyear/Settings/ProjectSettings.h"
#include "Lightyear/Settings/SettingsManager.h"

LY_DISABLE_WARNINGS_PUSH
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
LY_DISABLE_WARNINGS_POP

#include <iostream>

namespace ly
{

ref<spdlog::logger> Log::m_sCoreLogger;
ref<spdlog::logger> Log::m_sClientLogger;

constexpr std::string_view k_logRelativePath{"log/lightyear.log"};

void Log::init()
{
    // std::filesystem::path savedDirectory = SettingsManager::Project().savedDirectory;

    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(makeRef<spdlog::sinks::stdout_color_sink_mt>());
    // logSinks.emplace_back(
    //     MakeRef<spdlog::sinks::basic_file_sink_mt>((savedDirectory / kLogRelativePath).string(), true));

    logSinks[0]->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e %z] [%n] [%l] [thread %t] %v%$");
    // logSinks[1]->set_pattern("%^[%Y-%m-%d %H:%M:%S %z] [%n] [%l] [thread %t] %v%$");

    m_sCoreLogger = makeRef<spdlog::logger>("LIGHTYEAR", logSinks.begin(), logSinks.end());
    spdlog::register_logger(m_sCoreLogger);
    m_sCoreLogger->set_level(spdlog::level::trace);
    m_sCoreLogger->flush_on(spdlog::level::trace);

    m_sClientLogger = makeRef<spdlog::logger>("APP", logSinks.begin(), logSinks.end());
    spdlog::register_logger(m_sClientLogger);
    m_sClientLogger->set_level(spdlog::level::trace);
    m_sClientLogger->flush_on(spdlog::level::trace);

    spdlog::set_error_handler([](std::string const& msg) {
        std::cerr << "[SPDLOG ERROR] " << msg << '\n';
        std::ofstream fallbackLog("log/error_fallback.log", std::ios::app);
        fallbackLog << "[SPDLOG ERROR] " << msg << '\n';
    });
}

void Log::shutdown() { spdlog::shutdown(); }

} // namespace ly
