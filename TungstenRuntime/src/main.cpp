#include "TungstenCore/TungstenCore.hpp"
#include "generated/componentDeclorations.hpp"
#include "generated/componentIncludes.hpp"

// Forward declorations
//#define W_COMPONENT_X(Type, Name, Include) class Type;
//#include TUNGSTEN_PROJECT_COMPONENT_LIST_INCLUDE
//#undef W_COMPONENT_X

// Includes
//#define W_COMPONENT_X(Type, Name, Include) #include Include
//#include TUNGSTEN_PROJECT_COMPONENT_LIST_INCLUDE
//#undef W_COMPONENT_X

int main()
{
    wCore::Application app;
    return app.Run().exitCode;
}