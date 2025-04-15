#pragma once

#include "Lightyear/LightyearAPI.h"
#include <lypch.h>
#include <spdlog/spdlog.h>

namespace ly {

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
#define LY_CORE_INFO(...) ly::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LY_CORE_WARN(...) ly::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LY_CORE_ERROR(...) ly::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LY_CORE_TRACE(...) ly::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LY_CORE_FATAL(...) ly::Log::GetCoreLogger()->critical(__VA_ARGS__)

// All Client Logs
#define LY_CLIENT_INFO(...) ly::Log::GetClientLogger()->info(__VA_ARGS__)
#define LY_CLIENT_WARN(...) ly::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LY_CLIENT_ERROR(...) ly::Log::GetClientLogger()->error(__VA_ARGS__)
#define LY_CLIENT_TRACE(...) ly::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LY_CLIENT_FATAL(...) ly::Log::GetClientLogger()->critical(__VA_ARGS__)
