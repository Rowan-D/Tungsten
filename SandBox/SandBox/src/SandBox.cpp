#include "SandBox/SandBox.hpp"

namespace SandBox {
    class Transform
    {
    public:
        Transform()
        {
            W_DEBUG_LOG_INFO("{} Transform Constructor Called!!!", ANSI_RED);
        }

        ~Transform()
        {
            W_DEBUG_LOG_INFO("{} Transform Destructor Called!!!", ANSI_RED);
        }

    private:
        float matrix[4 * 4];
    };

    class GameManager
    {
    public:
        GameManager()
        {
            W_DEBUG_LOG_INFO("{} GameManager Constructor Called!!!!!!", ANSI_BLUE);
        }

        ~GameManager()
        {
            W_DEBUG_LOG_INFO("{} GameManager Destructor Called!!!!!!", ANSI_BLUE);
        }

    private:
        int i;
    };

    void Awake(wCore::ComponentSetup& componentManager)
    {
        W_DEBUG_LOG_INFO("Hello, World! From SandBox Awake!");

        componentManager.Add<Transform>("Transform");
        componentManager.Add<GameManager>("GameManager");
    }
}