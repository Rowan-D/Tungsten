#include "TungstenLogger.hpp"

namespace wUtils
{
    Logger& Logger::Get()
    {
        static Logger instance;
        return instance;
    }
}