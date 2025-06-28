#ifndef TUNGSTEN_UTILS_TUNGSTEN_LOGGER_HPP
#define TUNGSTEN_UTILS_TUNGSTEN_LOGGER_HPP

#include <iostream>
#include "fmt/format.h"

#define W_DEBUG_LOG_INFO(...) wUtils::TungstenLogger::GetDebugLogger().LogInfo(__VA_ARGS__)
#define W_DEBUG_LOG_WARNING(...) wUtils::TungstenLogger::GetDebugLogger().LogWarning(__VA_ARGS__)
#define W_DEBUG_LOG_ERROR(...) wUtils::TungstenLogger::GetDebugLogger().LogError(__VA_ARGS__)
#define W_DEBUG_LOG_CRITICAL_ERROR(...) wUtils::TungstenLogger::GetDebugLogger().LogCritical(__VA_ARGS__)

#define W_LOG_INFO(logger, ...) logger.LogInfo(__VA_ARGS__)
#define W_LOG_WARNING(logger, ...) logger.LogWarning(__VA_ARGS__)
#define W_LOG_ERROR(logger, ...) logger.LogError(__VA_ARGS__)
#define W_LOG_CRITICAL_ERROR(logger, ...) logger.LogCritical(__VA_ARGS__)

namespace wUtils
{
    class TungstenLogger
    {
    public:
        template<typename... Args>
        void LogInfo(fmt::format_string<Args...> fmt, Args&&... args)
        {
            std::cout << "[Info] " << fmt::format(fmt, std::forward<Args>(args)...) << std::endl;
        }

        template<typename... Args>
        void LogWarning(fmt::format_string<Args...> fmt, Args&&... args)
        {
            std::cout << "[Warning] " << fmt::format(fmt, args...) << std::endl;
        }

        template<typename... Args>
        void LogError(fmt::format_string<Args...> fmt, Args&&... args)
        {
            std::cerr << "[Error] " << fmt::format(fmt, args...) << std::endl;
        }

        template<typename... Args>
        void LogCritical(fmt::format_string<Args...> fmt, Args&&... args)
        {
            std::cerr << "[Error] " << fmt::format(fmt, args...) << std::endl;
        }

        static TungstenLogger& GetDebugLogger();
    private:
    };
}

#endif