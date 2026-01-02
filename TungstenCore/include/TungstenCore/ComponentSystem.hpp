#ifndef TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP
#define TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP

#include "TungstenUtils/wIndex.hpp"
#include <optional>

namespace wCore
{
    class Application;

    using SceneIndex = wIndex;
    inline constexpr SceneIndex InvalidSceneIndex = 0;
    inline constexpr SceneIndex SceneIndexStart = 1;

    using ComponentIndex = wIndex;
    inline constexpr ComponentIndex InvalidComponentIndex = 0;
    inline constexpr ComponentIndex ComponentIndexStart = 1;

    class ComponentGeneration
    {
    public:
        constexpr ComponentGeneration() noexcept
            : m_generation(0) {}

        friend constexpr bool operator==(const ComponentGeneration&, const ComponentGeneration&) = default;

    private:
        uint32_t m_generation;
        friend class ComponentSystem;
    };
    
    class SceneGeneration
    {
    public:
        constexpr SceneGeneration() noexcept
            : m_generation(0) {}

        friend constexpr bool operator==(const SceneGeneration&, const SceneGeneration&) = default;

    private:
        uint32_t m_generation;
        friend class ComponentSystem;
    };
    
    class SceneHandle
    {
        SceneIndex sceneIndex;
        SceneGeneration generation;
    };

    template<typename T>
    class ComponentHandle
    {
        SceneHandle sceneHandle;
        ComponentIndex componentIndex;
        ComponentGeneration generation;
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

        struct ComponentListHeaderHot
        {
            void* dense;
            ComponentIndex* slotToDense;
            ComponentGeneration* generations;
        };

        struct ComponentListHeaderCold
        {
            wIndex slotCount;
            wIndex denceCount;
            wIndex capacity;
            wUtils::RelocatableFreeListHeader<ComponentIndex> freeList;
        };

        struct PageListHeaderHot
        {
            void* data;
            ComponentGeneration* generations;
        };

        struct PageListHeaderCold
        {
            wIndex slotCount;
            wIndex pageCount;
            wUtils::RelocatableFreeListHeader<ComponentIndex> freeList;
        };

        void ReallocateScenes(wIndex newCapacity);

        Application& m_app;

        wIndex m_sceneCapacity;
    };
}

#endif