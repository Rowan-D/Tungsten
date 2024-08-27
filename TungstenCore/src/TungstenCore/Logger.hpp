#pragma once
#include "core.hpp"

namespace wCore
{
    class Logger
    {
    public:
        Logger();

        static Logger& Get();

        inline std::shared_ptr<spdlog::logger>& GetLogger() { return m_console; }

    private:
        std::shared_ptr<spdlog::logger> m_console;
    };
}