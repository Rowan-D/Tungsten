#pragma once
#include "core.hpp"
#include <iostream>

namespace wCore
{
    class Loggerasfd
    {
    public:
        Loggerasfd();

        static Loggerasfd& Get();

        inline std::shared_ptr<spdlog::logger>& GetLogger() { return m_console; }

    private:
        std::shared_ptr<spdlog::logger> m_console;
    };
}