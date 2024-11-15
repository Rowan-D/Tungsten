#pragma once
#include "wForgePCH.hpp"
#include "ErrorList.hpp"
#include "SceneData.hpp"
#include "utils/pathUtility.hpp"

namespace wForge
{
    class TungstenForge
    {
    public:
        TungstenForge();

        std::optional<std::filesystem::path> GetProjectPath(const std::filesystem::path& inputPath);
        bool BuildProject(const std::filesystem::path& projectPath, std::filesystem::path outputDir);

        std::string ReadFile(const std::filesystem::path& path);

        ErrorList errorList;

    private:
        std::optional<std::filesystem::path> GetProjectPathImpl(const std::filesystem::path& inputPath);
        bool BuildProjectImpl(const std::filesystem::path& projectPath, const std::filesystem::path& outputDir);
        std::string ReadFileImpl(const std::filesystem::path& path);

        std::optional<std::vector<uint8_t>> SceneDataToSceneBinary(const SceneData& sceneData);
    };
}
