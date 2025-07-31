#include "SandBox/SandBox.hpp"

namespace SandBox {
    class Transform
    {
        float matrix[4 * 4];
    };

    class GameManager
    {
        int i;
    };

    void Awake(wCore::ComponentSetup& componentManager)
    {
        W_DEBUG_LOG_INFO("Hello, World! From SandBox Awake!");

        componentManager.Add<Transform>("Transform");
        componentManager.Add<GameManager>("GameManager");
    }
}