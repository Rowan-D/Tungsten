#include <cstdint>
#include <iostream>
#include <string>

#include "TungsrenForge.hpp"

int main(int argc, char* argv[])
{
    wForge::TungsrenForge projectBuilder;
    projectBuilder.BuildProject("C:/Users/Rowan/Desktop/SandBox", "C:/Users/Rowan/Desktop");
    
    std::cout << "----------------------------" << std::endl;
    
    //if (errorList.size() > 0)
    //{
    //    std::cout << ANSI_BRIGHT_RED << "Error Count: " << errorList.size() << ANSI_RESET << std::endl;
    //
    //    //for (const wForge::BuildError& error : errorList)
    //    //{
    //    //    std::cout << std::format("{}: {}", wForge::BuildError::ErrorTypeToString(error.type), error.message) << std::endl;
    //    //}
    //}
}