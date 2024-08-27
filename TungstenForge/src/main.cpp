#include <cstdint>
#include <iostream>
#include <string>

#include "TungsrenForge.hpp"

int main(int argc, char* argv[])
{
    std::vector<wForge::BuildError> errorList;
    wForge::BuildProject("C:/Users/Rowan/Desktop/SandBox", "C:/Users/Rowan/Desktop", errorList);
    
    std::cout << "----------------------------" << std::endl;
    
    if (errorList.size() > 0)
    {
        std::cout << ANSI_BRIGHT_RED << "Error Count: " << errorList.size() << ANSI_RESET << std::endl;

        //for (const wForge::BuildError& error : errorList)
        //{
        //    std::cout << std::format("{}: {}", wForge::BuildError::ErrorTypeToString(error.type), error.message) << std::endl;
        //}
    }
}