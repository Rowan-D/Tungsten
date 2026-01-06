#include "TungstenCore/TungstenCore.hpp"
#include "generated/componentIncludes.hpp"

int main()
{
    wCore::Application app;

    wCore::ComponentSystem& cs = app.GetComponentSystem();

    wCore::SceneHandle mainScene = cs.CreateScene();
    cs.ReserveComponents<Project::Player>(mainScene.sceneIndex, 5);

    return app.Run().exitCode;
}