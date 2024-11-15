#pragma once
#include "wForgePCH.hpp"

namespace wForge
{
    std::filesystem::path GetHomeDirectory();
    std::filesystem::path ExpandTilde(const std::string_view& path);
}