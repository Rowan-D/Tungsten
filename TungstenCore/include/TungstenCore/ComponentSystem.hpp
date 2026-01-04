#ifndef TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP
#define TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP

#include "ComponentSetup.hpp"
#include "TungstenUtils/wIndex.hpp"
#include <optional>

namespace wCore
{
    class Application;

    using SceneIndex = wIndex;
    inline constexpr SceneIndex invalidSceneIndex = 0;
    inline constexpr SceneIndex sceneIndexStart = 1;

    using ComponentIndex = wIndex;
    inline constexpr ComponentIndex invalidComponentIndex = 0;
    inline constexpr ComponentIndex componentIndexStart = 1;

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
    
    struct SceneHandle
    {
        SceneHandle(SceneIndex a_sceneIndex, SceneGeneration a_generation)
            : sceneIndex(a_sceneIndex), generation(a_generation) {}

        SceneIndex sceneIndex;
        SceneGeneration generation;
    };

    template<typename T>
    struct ComponentHandle
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
        inline void ReserveSceneFreeList(wIndex minCapacity) { m_sceneFreeList.Reserve(minCapacity); }
        [[nodiscard]] SceneHandle CreateScene();
        [[nodiscard]] inline bool SceneExists(SceneHandle sceneHandle) const noexcept { return sceneIndex - sceneIndexStart < m_sceneSlotCount && sceneHandle.generation == m_sceneSlots[sceneHandle.sceneIndex - sceneIndexStart].generation; }
        void DestroyScene(SceneIndex sceneIndex) noexcept;
        void DestroyScene(SceneHandle sceneHandle) noexcept;

        [[nodiscard]] inline wIndex GetSceneCount() const noexcept { return m_sceneSlotCount - m_sceneFreeList.Count(); }
        [[nodiscard]] inline wIndex GetSceneCapacity() const noexcept { return m_sceneCapacity; }
        [[nodiscard]] inline wIndex GetSceneFreeListCount() const noexcept { return m_sceneFreeList.Count(); }
        [[nodiscard]] inline wIndex GetSceneFreeListCapacity() const noexcept { return m_sceneFreeList.Capacity(); }
        [[nodiscard]] inline wIndex GetSceneSlotCount() const noexcept { return m_sceneSlotCount; }

        template<typename T>
        void ReserveComponents(SceneIndex sceneIndex, wIndex minCapacity)
        {
            const SceneSlot& sceneSlot = m_sceneSlots[sceneIndex - sceneIndexStart];
            if constexpr (componentPageSize_v<T>)
            {
                const std::size_t pageListHeaderIndex = sceneSlot.denseIndex * componentSetup.pageListCount + componentListIndex_v<T>;
                PageListHeaderCold& pageListHeaderCold = m_pageListsCold[pageListHeaderIndex];
                if (minCapacity > pageListHeaderCold.pageCount * componentPageSize_v<T>)
                {
                    const wIndex minPageCount = wUtils::IntDivCeil(minCapacity, componentPageSize_v<T>);
                    ReallocatePages<T>(m_pageListsHot[pageListHeaderIndex], pageListHeaderCold, minPageCount);
                }
            }
            else
            {
                const std::size_t componentListHeaderIndex = sceneSlot.denseIndex * componentSetup.slotListCount + componentListIndex_v<T>;
                ComponentListHeaderCold& componentListHeaderCold = m_componentListsCold[componentListHeaderIndex];
                if (minCapacity > componentListHeaderCold.capacity)
                {
                    ReallocateComponents<T>(m_componentListsHot[componentListHeaderIndex], componentListHeaderCold, minCapacity);
                }
            }
        }

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

        struct SceneSlot
        {
            SceneSlot(SceneIndex a_denseIndex)
                : denseIndex(a_denseIndex), generation() {}

            SceneIndex denseIndex;
            SceneGeneration generation;
        };

        struct ComponentListHeaderHot
        {
            void* data;
        };

        struct PageListHeaderHot
        {
            void* data;
        };
        
        struct ComponentListHeaderCold
        {
            wIndex slotCount;
            wIndex slotGenerationCount;
            wIndex denceCount;
            wIndex capacity;
            wUtils::RelocatableFreeListHeader<ComponentIndex> freeList;
        };

        struct PageListHeaderCold
        {
            wIndex slotCount;
            wIndex slotGenerationCount;
            wIndex pageCount;
            wUtils::RelocatableFreeListHeader<ComponentIndex> freeList;
        };

        struct SceneData
        {
            wIndex nameIndex;
        };

        void ReallocateScenes(wIndex newCapacity);

        template<typename T>
        static void ReallocatePages(PageListHeaderHot& headerHot, PageListHeaderCold& headerCold, wIndex newPageCount)
        {T*
            std::size_t offset = newPageCount * sizeof(T*);
            
            offset = wUtils::AlignUp(offset, alignof(ComponentGeneration));
            const std::size_t componentGenerationOffset = offset;
            offset += componentPageSize_v<T> * sizeof(ComponentGeneration) * newPageCount;

            constexpr std::size_t alignment = wUtils::Max(alignof(T*), alignof(ComponentGeneration));

            if (headerHot.data)
            {
                std::byte* newMemory = static_cast<std::byte*>(
                    ::operator new(offset, std::align_val_t(alignment))
                );

                std::memcpy(newMemory, headerHot.data, headerCold.pageCount * sizeof(T*));
                ::operator delete(headerHot.data, std::align_val_t(alignof(T*)));

                if (headerCold.slotCount)
                {
                    std::memcpy(reinterpret_cast<ComponentGeneration*>(newMemory + componentGenerationOffset), reinterpret_cast<ComponentGeneration*>(headerHot.data + componentGenerationOffset), headerCold.slotCount * sizeof(ComponentGeneration));
                }
                
                headerHot.data = newMemory;
            }
            else
            {
                headerHot.data = ::operator new(newPageCount * sizeof(T*), std::align_val_t(alignment));
            }
            for (wIndex pageIndex = headerCold.pageCount; pageIndex < newPageCount; ++pageIndex)
            {
                newMemory[pageIndex] = static_cast<T*>(
                    ::operator new(PageSize * sizeof(T), std::align_val_t(alignof(T)))
                );
            }

            headerCold.pageCount = newPageCount;
        }

        template<typename T>
        static void ReallocateComponents(ComponentListHeaderHot& headerHot, ComponentListHeaderCold& headerCold, wIndex newCapacity)
        {
            std::size_t offset = newCapacity * sizeof(T);

            offset = wUtils::AlignUp(offset, alignof(ComponentIndex));
            const std::size_t slotToDenceOffset = offset;
            offset += newCapacity * sizeof(ComponentIndex);

            offset = wUtils::AlignUp(offset, alignof(ComponentGeneration));
            const std::size_t componentGenerationOffset = offset;
            offset += newCapacity * sizeof(ComponentGeneration);

            constexpr std::size_t alignment = wUtils::Max(alignof(T), alignof(ComponentIndex), alignof(ComponentGeneration));


            if (headerHot.data)
            {
                std::byte* newMemory = static_cast<std::byte*>(
                    ::operator new(offset, std::align_val_t(alignment))
                );

                ComponentIndex* newSlotToDense = reinterpret_cast<ComponentIndex*>(newMemory + slotToDenceOffset);
                ComponentGeneration* newComponentGenerations = reinterpret_cast<ComponentGeneration*>(newMemory + componentGenerationOffset);

                if constexpr (std::is_trivially_copyable_v<T>)
                {
                    if (headerCold.denseCount)
                    {
                        std::memcpy(newMemory, headerHot.dense, headerCold.denseCount * sizeof(T));
                    }
                }
                else
                {
                    T* const begin = static_cast<T*>(headerHot.dense);
                    T* end = begin + headerCold.denseCount;
                    T* dst = newMemory;
                    for (T* src = begin; src != end; ++src, ++dst)
                    {
                        std::construct_at(dst, std::move(*src));
                        if constexpr (!std::is_trivially_destructible_v<T>)
                        {
                            std::destroy_at(src);
                        }
                    }
                }
                if (headerCold.slotCount)
                {
                    std::memcpy(newMemory, headerHot.dense, headerCold.slotCount * sizeof(ComponentIndex));
                }
                ::operator delete(headerHot.dense, std::align_val_t(alignment));
            }
            else
            {
                headerHot.data = ::operator new(offset, std::align_val_t(alignment));
            }

            headerHot.dense = newMemory;
            headerHot.slotToDense = reinterpret_cast<ComponentIndex*>(newMemory + slotToDenceOffset);
            headerHot.componentGenerations = reinterpret_cast<ComponentGeneration*>(newMemory + componentGenerationOffset);

            headerCold.capacity = newCapacity;
        }
        
        Application& m_app;

        std::byte* m_scenes;
        SceneIndex* m_sceneSlots;
        ComponentListHeaderHot* m_componentListsHot;
        PageListHeaderHot* m_pageListsHot;
        ComponentListHeaderCold* m_componentListsCold;
        PageListHeaderCold* m_pageListsCold;
        SceneData* m_sceneData;
        wIndex m_sceneSlotCount;
        wIndex m_sceneSlotGenerationCount;
        wIndex m_sceneDenseCount;
        wIndex m_sceneCapacity;

        wUtils::FreeList<SceneIndex> m_sceneFreeList;
    };
}

#endif