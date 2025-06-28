#ifndef TUNGSTEN_FORGE_TUNGSTEN_FORGE_HPP
#define TUNGSTEN_FORGE_TUNGSTEN_FORGE_HPP

#include "TungstenUtils/TungstenUtils.hpp"
#include <filesystem>

namespace wForge
{
    class TungstenForge
    {
    public:
        TungstenForge();

        bool BuildProject(const std::filesystem::path& projectPath, std::filesystem::path outputDir);

        wUtils::TungstenLogger errorList;

    private:
    };
}

#endif