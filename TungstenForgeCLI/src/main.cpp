#include "TungstenForge/TungstenForge.hpp"

int main(int argc, char** argv)
{
    wUtils::TungstenLogger logger;
    if (argc < 2)
    {
        logger.LogError("Expected a path to a Tungsten Project!");
        return 1;
    }
    if (argc > 3)
    {
        logger.LogWarning("Expected up to 2 Arguments.");
        return 1;
    }

    const std::filesystem::path inputPath = argv[1];
    std::filesystem::path outputPath;
    if (argc > 2)
    {
        outputPath = argv[2];
    }
    else
    {
        outputPath = std::filesystem::current_path();
    }

    W_DEBUG_LOG_INFO("Input Path: {}", inputPath.string());
    W_DEBUG_LOG_INFO("Output Path: {}", outputPath.string());

    wForge::TungstenForge tungstenForge;
    if (tungstenForge.BuildProject(inputPath, outputPath, outputPath))
    {
        return 0;
    }

    return 1;
}