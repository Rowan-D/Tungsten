#pragma once
#include "Logger.hpp"
#include "macros.hpp"

#define W_LOGGING 1
#if W_LOGGING

#define W_LOG_TRACE_NO_LOCATION(...) wCore::Loggerasfd::Get().GetLogger()->trace(__VA_ARGS__)
#define W_LOG_DEBUG_NO_LOCATION(...) wCore::Loggerasfd::Get().GetLogger()->debug(__VA_ARGS__)
#define W_LOG_INFO_NO_LOCATION(...) wCore::Loggerasfd::Get().GetLogger()->info(__VA_ARGS__)
#define W_LOG_WARNING_NO_LOCATION(...) wCore::Loggerasfd::Get().GetLogger()->warn(__VA_ARGS__)
#define W_LOG_ERROR_NO_LOCATION(...) wCore::Loggerasfd::Get().GetLogger()->error(__VA_ARGS__)
#define W_LOG_CRITICAL_ERROR_NO_LOCATION(...) wCore::Loggerasfd::Get().GetLogger()->critical(__VA_ARGS__); DEBUG_BREAK()

#define W_LOG_TRACE(...) wCore::Loggerasfd::Get().GetLogger()->trace("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__)
#define W_LOG_DEBUG(...) wCore::Loggerasfd::Get().GetLogger()->debug("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__)
#define W_LOG_INFO(...) wCore::Loggerasfd::Get().GetLogger()->info("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__)
#define W_LOG_WARNING(...) wCore::Loggerasfd::Get().GetLogger()->warn("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__)
#define W_LOG_ERROR(...) wCore::Loggerasfd::Get().GetLogger()->error("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__)
#define W_LOG_CRITICAL_ERROR(...) wCore::Loggerasfd::Get().GetLogger()->critical("{} {} {}", fmt::format(__VA_ARGS__), __FUNCTION__, __LINE__); DEBUG_BREAK()
/*
#define W_LOG_TRACE_NO_LOCATION(...) wCore::Loggerasfd::Get().Test2();
#define W_LOG_DEBUG_NO_LOCATION(...) wCore::Loggerasfd::Get().Test2();
#define W_LOG_INFO_NO_LOCATION(...) wCore::Loggerasfd::Get().Test2();
#define W_LOG_WARNING_NO_LOCATION(...) wCore::Loggerasfd::Get().Test2();
#define W_LOG_ERROR_NO_LOCATION(...) wCore::Loggerasfd::Get().Test2();
#define W_LOG_CRITICAL_ERROR_NO_LOCATION(...) wCore::Loggerasfd::Get().Test2();

#define W_LOG_TRACE(...) wCore::Loggerasfd::Get().Test2();
#define W_LOG_DEBUG(...) wCore::Loggerasfd::Get().Test2();
#define W_LOG_INFO(...) wCore::Loggerasfd::Get().Test2();
#define W_LOG_WARNING(...) wCore::Loggerasfd::Get().Test2();
#define W_LOG_ERROR(...) wCore::Loggerasfd::Get().Test2();
#define W_LOG_CRITICAL_ERROR(...) wCore::Loggerasfd::Get().Test2();
*/

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