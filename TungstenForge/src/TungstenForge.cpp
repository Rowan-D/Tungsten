#include "TungstenForge/TungstenForge.hpp"
#include <fstream>

namespace wForge
{

    TungstenForge::TungstenForge()
        : errorList()
    {

    }

    bool TungstenForge::BuildProject(const std::filesystem::path& projectPath, const std::filesystem::path& outputIntDir, std::filesystem::path& outputBuildDir)
    {
        namespace fs = std::filesystem;

        W_LOG_INFO(errorList, "Build called.");
        W_LOG_INFO(errorList, "Project path: {}", projectPath.string());
        W_LOG_INFO(errorList, "Output Intermediate Directory: {}", outputIntDir.string());
        W_LOG_INFO(errorList, "Output Build Directory: {}", outputBuildDir.string());

        fs::create_directory(outputIntDir / "Intermediate");

        fs::copy("res/TungstenRuntime", outputIntDir / "Intermediate/TungstenRuntime", fs::copy_options::recursive);

        fs::create_directory(outputIntDir / "Intermediate/build");
        fs::create_directory(outputBuildDir / "Build");
        return true;
    }
}