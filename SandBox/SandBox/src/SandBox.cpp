#include "SandBox/SandBox.hpp"

namespace SandBox {
    class Transform
    {
        float matrix[4 * 4];
    };

    void Awake(wCore::ComponentManager& componentManager)
    {
        W_DEBUG_LOG_INFO("Hello, World! From SandBox Awake!");

        componentManager.Add<Transform>();
    }
}