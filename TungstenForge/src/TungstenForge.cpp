#include "TungstenForge/TungstenForge.hpp"

namespace wForge
{
    TungstenForge::TungstenForge()
        : errorList()
    {

    }

    bool TungstenForge::BuildProject(const std::filesystem::path& projectPath, std::filesystem::path outputDir)
    {
        W_LOG_INFO(errorList, "Build called. Asset path: {} Output path: {}", projectPath.string(), outputDir.string());




        return true;
    }
}