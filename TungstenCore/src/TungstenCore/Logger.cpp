#include "wCorePCH.hpp"
#include "Logger.hpp"
#include <iostream>

#define W_LOGGER_ERROR(x) std::cerr << ANSI_BOLD << ANSI_BG_RED << ANSI_WHITE << "[LOGGER ERROR!!]" << ANSI_RESET << ANSI_BRIGHT_RED << ' ' << x << ANSI_RESET << std::endl; __debugbreak()

namespace wCore
{
    Logger::Logger()
        : m_console(nullptr)
    {
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        stdout_sink->set_level(spdlog::level::trace);  // Set log level for stdout sink
        stdout_sink->set_pattern("%^[%l]%$ %v");

        //auto stderr_sink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        //stderr_sink->set_level(spdlog::level::err);  // Set log level for stderr sink
        //stderr_sink->set_pattern("%^[%l]%$ %v");

        std::vector<spdlog::sink_ptr> sinks{ stdout_sink/*, stderr_sink*/};
        m_console = std::make_shared<spdlog::logger>("console", sinks.begin(), sinks.end());
        m_console->set_level(spdlog::level::trace);  // Set the logger's global log level
        spdlog::register_logger(m_console);

#ifdef _WIN32
        // Windows platform: Set color using uint16_t value
        stdout_sink->set_color(spdlog::level::info, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#else
        // Non-Windows platform: Set color using ANSI escape sequence
        stdout_sink->set_color(spdlog::level::info, "\033[1;34m"); // ANSI escape sequence for bright blue
#endif

        m_console->set_level(spdlog::level::trace);
    }

    Logger& Logger::Get()
    {
        static Logger instance;
        return instance;
    }
}