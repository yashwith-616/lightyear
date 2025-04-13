#pragma once

#include "Lightyear/LightyearAPI.h"
#include <lypch.h>
#include <spdlog/spdlog.h>

namespace Lightyear::utils {

class LIGHTYEAR_API Log {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};
}

// All Core logs
#define LY_CORE_INFO(...) Lightyear::utils::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LY_CORE_WARN(...) Lightyear::utils::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LY_CORE_ERROR(...) Lightyear::utils::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LY_CORE_TRACE(...) Lightyear::utils::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LY_CORE_FATAL(...) Lightyear::utils::Log::GetCoreLogger()->critical(__VA_ARGS__)

// All Client Logs
#define LY_CLIENT_INFO(...) Lightyear::utils::Log::GetClientLogger()->info(__VA_ARGS__)
#define LY_CLIENT_WARN(...) Lightyear::utils::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LY_CLIENT_ERROR(...) Lightyear::utils::Log::GetClientLogger()->error(__VA_ARGS__)
#define LY_CLIENT_TRACE(...) Lightyear::utils::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LY_CLIENT_FATAL(...) Lightyear::utils::Log::GetClientLogger()->critical(__VA_ARGS__)
