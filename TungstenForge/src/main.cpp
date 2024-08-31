#include <cstdint>
#include <iostream>
#include <string>

#include "TungsrenForge.hpp"

int main(int argc, char* argv[])
{
    wForge::TungsrenForge projectBuilder;
    projectBuilder.BuildProject("C:/Users/Rowan/Desktop/SandBox", "C:/Users/Rowan/Desktop");
    
    std::cout << "----------------------------" << std::endl;
    
    std::vector<wForge::ErrorList::Error> errorList = projectBuilder.errorList.errorList;
    if (errorList.size() > 0)
    {
        std::cout << ANSI_BRIGHT_RED << "Error Count: " << errorList.size() << ANSI_RESET << std::endl;
    
        for (const wForge::ErrorList::Error& error : errorList)
        {
            std::cout << std::format("{}: {}", error.SeverityToString(), error.message) << std::endl;
        }
    }
}