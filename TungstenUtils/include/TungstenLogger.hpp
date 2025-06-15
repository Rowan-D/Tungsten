#ifndef TUNGSTEN_UTILS_TUNGSTEN_LOGGER_HPP
#define TUNGSTEN_UTILS_TUNGSTEN_LOGGER_HPP

#include "fmt/format.h"
#include <iostream>

#define W_LOG_INFO(...) wUtils::Logger::Get().LogInfo(__VA_ARGS__);
#define W_LOG_WARNING(...) wUtils::Logger::Get().LogWarning(__VA_ARGS__);
#define W_LOG_ERROR(...) wUtils::Logger::Get().LogError(__VA_ARGS__);
#define W_LOG_CRITICAL_ERROR(...) wUtils::Logger::Get().LogCritical(__VA_ARGS__);

namespace wUtils
{
    class Logger
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

        static Logger& Get();
    private:
    };
}

#endif