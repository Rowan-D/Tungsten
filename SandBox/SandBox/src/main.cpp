#include "TungstenCore/TungstenCore.hpp"

class Transform
{
    float matrix[4 * 4];
};

int Awake()
{
    W_DEBUG_LOG_INFO("Hello, World!");

    wCore::Application app;
    wCore::ComponentManager& componentManager = app.GetComponentManager();
    wCore::ComponentList<Transform> transformComponentList(componentManager);

    //app.CreateComponent<Transform>();

    return app.Run();
}