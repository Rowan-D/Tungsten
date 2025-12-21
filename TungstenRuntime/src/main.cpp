#include "TungstenCore/TungstenCore.hpp"
#include "generated/projectDefines.hpp"

// Forward declorations
#define W_COMPONENT_X(Type, Name, Include) class Type;
#include TUNGSTEN_PROJECT_COMPONENT_LIST_INCLUDE_PATH
#undef W_COMPONENT_X

// Includes
#define W_COMPONENT_X(Type, Name, Include) #include Include
#include TUNGSTEN_PROJECT_COMPONENT_LIST_INCLUDE_PATH
#undef W_COMPONENT_X

int main()
{
    wCore::Application app;
    return app.Run().exitCode;
}