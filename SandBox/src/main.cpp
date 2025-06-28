#include "TungstenCore/TungstenCore.hpp"

class Transform
{
    float matrix[4 * 4];
};

int main()
{
    W_DEBUG_LOG_INFO("Hello, World!");

    wCore::ComponentManager componentManager;
    wCore::ComponentList<Transform> transforms(componentManager);
}