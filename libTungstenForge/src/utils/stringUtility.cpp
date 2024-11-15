#include "wForgePCH.hpp"
#include "stringUtility.hpp"

namespace wForge
{
    std::string_view TrimString(const std::string_view& str)
    {
        std::string_view::const_iterator start = std::find_if_not(str.begin(), str.end(), ::isspace);
        std::string_view::const_iterator end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();

        // If the entire string is whitespace
        if (start >= end)
        {
            return {};
        }

        return std::string_view(&*start, static_cast<size_t>(end - start));
    }
}