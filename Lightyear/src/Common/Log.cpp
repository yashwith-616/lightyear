#include "Lightyear/Common/Log.h"

#include "Lightyear/Settings/ProjectSettings.h"
#include "Lightyear/Settings/SettingsManager.h"

LY_DISABLE_WARNINGS_PUSH
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
LY_DISABLE_WARNINGS_POP

#include <iostream>

namespace ly {

Ref<spdlog::logger> Log::s_CoreLogger;
Ref<spdlog::logger> Log::s_ClientLogger;

constexpr std::string_view kLogRelativePath{ "log/lightyear.log" };

void Log::Init() {
    // std::filesystem::path savedDirectory = SettingsManager::Project().savedDirectory;

    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(MakeRef<spdlog::sinks::stdout_color_sink_mt>());
    // logSinks.emplace_back(
    //     MakeRef<spdlog::sinks::basic_file_sink_mt>((savedDirectory / kLogRelativePath).string(), true));

    logSinks[0]->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e %z] [%n] [%l] [thread %t] %v%$");
    // logSinks[1]->set_pattern("%^[%Y-%m-%d %H:%M:%S %z] [%n] [%l] [thread %t] %v%$");

    s_CoreLogger = MakeRef<spdlog::logger>("LIGHTYEAR", logSinks.begin(), logSinks.end());
    spdlog::register_logger(s_CoreLogger);
    s_CoreLogger->set_level(spdlog::level::trace);
    s_CoreLogger->flush_on(spdlog::level::trace);

    s_ClientLogger = MakeRef<spdlog::logger>("APP", logSinks.begin(), logSinks.end());
    spdlog::register_logger(s_ClientLogger);
    s_ClientLogger->set_level(spdlog::level::trace);
    s_ClientLogger->flush_on(spdlog::level::trace);

    spdlog::set_error_handler([](const std::string& msg) {
        std::cerr << "[SPDLOG ERROR] " << msg << '\n';
        std::ofstream fallbackLog("log/error_fallback.log", std::ios::app);
        fallbackLog << "[SPDLOG ERROR] " << msg << '\n';
    });
}

void Log::Shutdown() {
    spdlog::shutdown();
}

}  // namespace ly
