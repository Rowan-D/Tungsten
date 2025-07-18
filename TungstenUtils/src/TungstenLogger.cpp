//#include "TungstenUtils/wUtilsPCH.hpp"
#include "TungstenUtils/TungstenLogger.hpp"

namespace wUtils
{
    TungstenLogger& TungstenLogger::GetDebugLogger()
    {
        static TungstenLogger instance;
        return instance;
    }
}