#include "wForgePCH.hpp"
#include "ErrorList.hpp"

namespace wForge
{
    ErrorList::Error::Error(Severity severity, ErrorCode code, const std::string& message)
        : severity(severity), code(code), message(message)
    {
    }

    ErrorList::ErrorList()
        : errorList()
    {
    }

    void ErrorList::Log(Severity severity, ErrorCode code, const std::string& message)
    {
        errorList.emplace_back(severity, code, message);

        switch (severity)
        {
        case Severity::Info:
            W_LOG_INFO_NO_LOCATION(" Code: {} {}", code, message);
            break;
        case Severity::Warning:
            W_LOG_WARNING_NO_LOCATION(" Code: {} {}", code, message);
            break;
        case Severity::Error:
            W_LOG_ERROR_NO_LOCATION(" Code: {} {}", code, message);
            break;
        default:
            W_LOG_CRITICAL_ERROR("Unknown ErrorType!");
            break;
        }
    }
}