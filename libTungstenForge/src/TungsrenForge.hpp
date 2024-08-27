#pragma once
#include "wForgePCH.hpp"

namespace wForge
{
    struct BuildError
    {
        enum Sevarity
        {
            Error,
            Warning
        };

        static inline const char* ErrorTypeToString(Sevarity type)
        {
            switch (type)
            {
            case Sevarity::Warning:
                return "Warning";
            case Sevarity::Error:
                return "Error";
            default:
                W_ASSERT(false, "Unknown ErrorType!");
                return "Unknown ErrorType!";
            }
        }

        BuildError(Sevarity type, const std::string& message, bool logError);
        BuildError(Sevarity type, std::string&& message, bool logError);

        void Log() const;

        Sevarity type;
        std::string message;
    };


    bool IsValedProjectPath(const std::filesystem::path& projectPath, std::string& errorMessage);
    bool IsValedOutputPath(const std::filesystem::path& outputDir, std::string& errorMessage);

    bool BuildProject(const std::filesystem::path& projectPath, const std::filesystem::path outputDir, std::vector<BuildError>& errorList);
}