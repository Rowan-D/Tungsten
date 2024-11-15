#include <cstdint>
#include <iostream>
#include <string>

#include "TungstenForge.hpp"

int main(int argc, char* argv[])
{
    wForge::TungstenForge projectBuilder;
    projectBuilder.BuildProject(wForge::ExpandTilde("~/dev/Tungsten/Sandbox"), wForge::ExpandTilde("~/dev/Tungsten/"));
    
    std::cout << "----------------------------" << std::endl;
    
    std::vector<wForge::ErrorList::Error> errorList = projectBuilder.errorList.errorList;
    if (errorList.size() > 0)
    {
        std::cout << ANSI_BRIGHT_RED << "Error Count: " << errorList.size() << ANSI_RESET << std::endl;
    
        for (const wForge::ErrorList::Error& error : errorList)
        {
            std::cout << error.SeverityToString() << ": " << error.message << std::endl;
        }
    }
}