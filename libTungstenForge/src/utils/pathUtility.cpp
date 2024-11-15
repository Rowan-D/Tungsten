#include "wForgePCH.hpp"
#include "pathUtility.hpp"
#include "stringUtility.hpp"

namespace wForge
{
    std::filesystem::path GetHomeDirectory()
    {
        #ifdef _WIN32
            const char* home = std::getenv("USERPROFILE");
        #else
            const char* home = std::getenv("HOME");
        #endif

        if (home)
        {
            return std::filesystem::path(home);
        }
        return {};
    }

    std::filesystem::path ExpandTilde(const std::string_view& path)
    {
        if (path.empty())
        {
            return {};
        }
        const std::string_view trimmedPath = TrimString(path);
        if (trimmedPath[0] == '~')
        {
            const std::filesystem::path homeDir = GetHomeDirectory();
            if (!homeDir.empty())
            {
                std::filesystem::path fullPath = homeDir / trimmedPath.substr(2);
                return fullPath.lexically_normal();
            }
        }
        return trimmedPath;
    }
}