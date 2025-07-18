#include "TungstenForge/TungstenForge.hpp"
#include <fstream>

namespace wForge
{

    TungstenForge::TungstenForge()
        : errorList()
    {

    }

    bool TungstenForge::BuildProject(const std::filesystem::path& projectPath, std::filesystem::path outputDir)
    {
        namespace fs = std::filesystem;

        W_LOG_INFO(errorList, "Build called. Asset path: {} Output path: {}", projectPath.string(), outputDir.string());

        fs::copy("res/TungstenRuntime", outputDir, fs::copy_options::recursive);

        fs::create_directory(outputDir / "Build");
        return true;
    }
}