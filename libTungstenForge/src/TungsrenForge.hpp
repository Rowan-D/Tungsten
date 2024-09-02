#pragma once
#include "wForgePCH.hpp"
#include "ErrorList.hpp"
#include "SceneData.hpp"

namespace wForge
{

    class TungsrenForge
    {
    public:
        TungsrenForge();

        static bool IsValedProjectPath(const std::filesystem::path& projectPath, std::string& errorMessage);
        static bool IsValedOutputPath(const std::filesystem::path& outputDir, std::string& errorMessage);

        bool BuildProject(const std::filesystem::path& projectPath, const std::filesystem::path outputDir);

        std::string ReadFile(const std::filesystem::path& path);

        ErrorList errorList;
    private:

        std::optional<std::filesystem::path> GetProjectPath(std::filesystem::path inputPath);

        std::optional<std::vector<uint8_t>> SceneDataToSceneBinary(const SceneData& sceneData);
    };
}