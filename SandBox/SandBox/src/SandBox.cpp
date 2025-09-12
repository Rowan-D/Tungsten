#include "SandBox/SandBox.hpp"

namespace SandBox {
    struct Transform {
        Transform() { W_DEBUG_LOG_INFO("{} Transform ctor", ANSI_RED); }
        Transform(const Transform& rhs) {
            W_DEBUG_LOG_INFO("{} Transform copy-ctor", ANSI_RED);
            std::memcpy(matrix, rhs.matrix, sizeof matrix);
        }
        Transform(Transform&& rhs) noexcept {
            // make move noexcept to satisfy your ECS noexcept policy
            try { W_DEBUG_LOG_INFO("{} Transform move-ctor", ANSI_RED); } catch (...) {}
            std::memcpy(matrix, rhs.matrix, sizeof matrix);
        }
        Transform& operator=(const Transform& rhs) {
            W_DEBUG_LOG_INFO("{} Transform copy-assign", ANSI_RED);
            std::memcpy(matrix, rhs.matrix, sizeof matrix);
            return *this;
        }
        Transform& operator=(Transform&& rhs) noexcept {
            try { W_DEBUG_LOG_INFO("{} Transform move-assign", ANSI_RED); } catch (...) {}
            std::memcpy(matrix, rhs.matrix, sizeof matrix);
            return *this;
        }
        ~Transform() {
            try { W_DEBUG_LOG_INFO("{} Transform dtor", ANSI_RED); } catch (...) {}
        }

        float matrix[16];
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

    struct TransformGorothPolicy
    {
        static constexpr wIndex InitialCapacity = 8;

        static inline constexpr wIndex Next(wIndex requested, wIndex current) noexcept
        {
            if (!current)
            {
                return std::max(InitialCapacity, requested);
            }
            return std::max(current * 2, requested);
        }
    };

    void Awake(wCore::ComponentSetup& ComponentSetup)
    {
        W_DEBUG_LOG_INFO("Hello, World! From SandBox Awake!");

        ComponentSetup.Add<Transform, 2048, TransformGorothPolicy>("Transform");
        ComponentSetup.Add<GameManager>("GameManager");
    }
}