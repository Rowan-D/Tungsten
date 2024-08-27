#pragma once
#include "Logger.hpp"

#define W_LOGGING 1
#ifdef W_LOGGING

#define W_LOG_TRACE_NO_LOCATION(...) wCore::Logger::Get().GetLogger()->trace(__VA_ARGS__)
#define W_LOG_DEBUG_NO_LOCATION(...) wCore::Logger::Get().GetLogger()->debug(__VA_ARGS__)
#define W_LOG_INFO_NO_LOCATION(...) wCore::Logger::Get().GetLogger()->info(__VA_ARGS__)
#define W_LOG_WARNING_NO_LOCATION(...) wCore::Logger::Get().GetLogger()->warn(__VA_ARGS__)
#define W_LOG_ERROR_NO_LOCATION(...) wCore::Logger::Get().GetLogger()->error(__VA_ARGS__)
#define W_LOG_CRITICAL_ERROR_NO_LOCATION(...) wCore::Logger::Get().GetLogger()->critical(__VA_ARGS__); __debugbreak()

#define W_LOG_TRACE(...) wCore::Logger::Get().GetLogger()->trace("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__)
#define W_LOG_DEBUG(...) wCore::Logger::Get().GetLogger()->debug("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__)
#define W_LOG_INFO(...) wCore::Logger::Get().GetLogger()->info("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__)
#define W_LOG_WARNING(...) wCore::Logger::Get().GetLogger()->warn("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__)
#define W_LOG_ERROR(...) wCore::Logger::Get().GetLogger()->error("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__)
#define W_LOG_CRITICAL_ERROR(...) wCore::Logger::Get().GetLogger()->critical("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__); __debugbreak()

#else

#define W_LOG_TRACE_NO_LOCATION(...)
#define W_LOG_DEBUG_NO_LOCATION(...)
#define W_LOG_INFO_NO_LOCATION(...)
#define W_LOG_WARNING_NO_LOCATION(...)
#define W_LOG_ERROR_NO_LOCATION(...)
#define W_LOG_CRITICAL_ERROR_NO_LOCATION(...)

#define W_LOG_TRACE(...)
#define W_LOG_DEBUG(...)
#define W_LOG_INFO(...)
#define W_LOG_WARNING(...)
#define W_LOG_ERROR(...)
#define W_LOG_CRITICAL_ERROR(...)

#endif