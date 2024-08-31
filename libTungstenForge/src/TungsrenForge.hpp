#pragma once
#include "wForgePCH.hpp"
#include "ErrorList.hpp"

namespace wForge
{

    class TungsrenForge
    {
    public:
        TungsrenForge();

        bool BuildProject(const std::filesystem::path& projectPath, const std::filesystem::path outputDir);

        static bool IsValedProjectPath(const std::filesystem::path& projectPath, std::string& errorMessage);
        static bool IsValedOutputPath(const std::filesystem::path& outputDir, std::string& errorMessage);

        ErrorList errorList;
    private:

        std::optional<std::filesystem::path> GetProjectPath(std::filesystem::path inputPath);

        std::optional<std::vector<uint8_t>> SceneToBytes(const std::filesystem::path& scenePath);
    };
}