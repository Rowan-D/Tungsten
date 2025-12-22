#ifndef TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP
#define TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP

#include "TungstenUtils/TungstenUtils.hpp"
#include <optional>

namespace wCore
{
    class Application;

    using SceneIndex = wIndex;
    using ComponentIndex = wIndex;

    class SceneHandle
    {
        SceneIndex sceneIndex;
    };

    template<typename T>
    class ComponentHandle
    {
        SceneHandle sceneHandle;
        ComponentIndex componentIndex;
    };

    class ComponentSystem
    {
    public:
        ComponentSystem(Application& app) noexcept;
        ~ComponentSystem() noexcept;

        void ReserveScenes(wIndex minCapacity);
        void ReserveSceneFreeList(wIndex minCapacity);
        [[nodiscard]] SceneHandle CreateScene();
        [[nodiscard]] bool SceneExists(SceneHandle sceneHandle) const noexcept;
        void DestroyScene(SceneIndex sceneIndex) noexcept;
        void DestroyScene(SceneHandle sceneHandle) noexcept;

        [[nodiscard]] wIndex GetSceneCount() const noexcept;
        [[nodiscard]] wIndex GetSceneCapacity() const noexcept;
        [[nodiscard]] wIndex GetSceneFreeListCount() const noexcept;
        [[nodiscard]] wIndex GetSceneFreeListCapacity() const noexcept;
        [[nodiscard]] wIndex GetSceneSlotCount() const noexcept;

        template<typename T>
        void ReserveComponents(SceneIndex sceneIndex, wIndex minCapacity);
        template<typename T>
        bool TryReserveComponents(SceneHandle sceneHandle, wIndex minCapacity);

        template<typename T>
        void ReserveComponentFreeList(SceneIndex sceneIndex, wIndex minCapacity);
        template<typename T>
        bool TryReserveComponentFreeList(SceneHandle sceneHandle, wIndex minCapacity);

        template<typename T>
        [[nodiscard]] ComponentHandle<T> CreateComponent(SceneIndex sceneIndex);
        template<typename T>
        [[nodiscard]] std::optional<ComponentHandle<T>> TryCreateComponent(SceneHandle sceneHandle);

        template<typename T>
        [[nodiscard]] bool ComponentExists(ComponentHandle<T> componentHandle) const noexcept;

        template<typename T>
        void DestroyComponent(SceneIndex sceneIndex, ComponentIndex componentIndex) noexcept;
        template<typename T>
        void DestroyComponent(SceneHandle sceneHandle, ComponentIndex componentIndex) noexcept;
        template<typename T>
        void DestroyComponent(ComponentHandle<T> componentHandle) noexcept;

        template<typename T>
        [[nodiscard]] T& GetComponent(SceneIndex sceneHandle, ComponentIndex componentIndex) noexcept;
        template<typename T>
        [[nodiscard]] T* TryGetComponent(SceneHandle sceneHandle, ComponentIndex componentIndex) noexcept;
        template<typename T>
        [[nodiscard]] T* TryGetComponent(ComponentHandle<T> componentHandle) noexcept;

        template<typename T>
        [[nodiscard]] const T& GetComponent(SceneIndex sceneIndex, ComponentIndex componentIndex) const noexcept;
        template<typename T>
        [[nodiscard]] const T* TryGetComponent(SceneHandle sceneHandle, ComponentIndex componentIndex) const noexcept;
        template<typename T>
        [[nodiscard]] const T* TryGetComponent(ComponentHandle<T> componentHandle) const noexcept;

        template<typename T>
        [[nodiscard]] wIndex GetComponentCount(SceneIndex sceneIndex) const noexcept;
        template<typename T>
        [[nodiscard]] wIndex GetComponentCapacity(SceneIndex sceneIndex) const noexcept;
        template<typename T>
        [[nodiscard]] wIndex GetComponentFreeListCount(SceneIndex sceneIndex) const noexcept;
        template<typename T>
        [[nodiscard]] wIndex GetComponentFreeListCapacity(SceneIndex sceneIndex) const noexcept;
        template<typename T>
        [[nodiscard]] wIndex GetComponentSlotCount(SceneIndex sceneIndex) const noexcept;

        template<typename T>
        [[nodiscard]] std::optional<wIndex> TryGetComponentCount(SceneHandle sceneHandle) const noexcept;
        template<typename T>
        [[nodiscard]] std::optional<wIndex> TryGetComponentCapacity(SceneHandle sceneHandle) const noexcept;
        template<typename T>
        [[nodiscard]] std::optional<wIndex> TryGetComponentFreeListCount(SceneHandle sceneHandle) const noexcept;
        template<typename T>
        [[nodiscard]] std::optional<wIndex> TryGetComponentFreeListCapacity(SceneHandle sceneHandle) const noexcept;
        template<typename T>
        [[nodiscard]] std::optional<wIndex> TryGetComponentSlotCount(SceneHandle sceneHandle) const noexcept;
        
        template<typename T>
        [[nodiscard]] const T* GetComponentDenseData(SceneIndex sceneIndex) const noexcept;
        template<typename T>
        [[nodiscard]] const T* TryGetComponentDenseData(SceneHandle sceneHandle) const noexcept;

    private:
        static constexpr wIndex InitialCapacity = 8;
        static inline constexpr wIndex CalculateNextCapacity(wIndex current) noexcept
        {
            if (current)
            {
                return current * 2;
            }
            return InitialCapacity;
        }

        Application& m_app;
    };
}

#endif