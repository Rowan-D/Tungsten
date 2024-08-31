#pragma once
#include "wForgePCH.hpp"

namespace wForge
{
    typedef uint32_t ErrorCode;

    constexpr ErrorCode ERROR_CODE_TODO = 0;

    enum class Severity
    {
        Info = 0,
        Warning = 1,
        Error = 2
    };

    inline const char* SeverityToString(Severity severity)
    {
        switch (severity)
        {
        case Severity::Warning:
            return "Warning";
        case Severity::Error:
            return "Error";
        default:
            W_LOG_CRITICAL_ERROR("Unknown ErrorType!");
            return "Unknown ErrorType!";
        }
    }

    class ErrorList
    {
    public:
        struct Error
        {
            Error(Severity severity, ErrorCode code, const std::string& message);

            inline const char* SeverityToString() const { return wForge::SeverityToString(severity); }

            Severity severity;
            ErrorCode code;
            std::string message;
        };

        ErrorList();

        void Log(Severity severity, ErrorCode code, const std::string& message);

        std::vector<Error> errorList;
    };
}


