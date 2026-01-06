#ifndef TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP
#define TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP

#include "ComponentSetup.hpp"
#include "TungstenUtils/wIndex.hpp"
#include "TungstenUtils/alignment.hpp"
#include "TungstenUtils/RelocatableFreeListHeader.hpp"
#include "TungstenUtils/FreeList.hpp"
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
        [[nodiscard]] inline bool SceneExists(SceneHandle sceneHandle) const noexcept { return sceneHandle.sceneIndex <= m_sceneSlotCount && sceneHandle.generation == m_sceneGenerations[sceneHandle.sceneIndex - sceneIndexStart]; }
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
            const SceneIndex sceneDenseIndex = m_sceneSlotToDense[sceneIndex - sceneIndexStart];
            if constexpr (componentPageSize_v<T>)
            {
                const wIndex pageListHeaderIndex = sceneDenseIndex * componentSetup.pageListCount + componentListIndex_v<T>;
                PageListHeaderCold& pageListHeaderCold = m_pageListsCold[pageListHeaderIndex];
                if (minCapacity > pageListHeaderCold.pageCount * componentPageSize_v<T>)
                {
                    ReallocatePages<T>(m_pageListsHot[pageListHeaderIndex], pageListHeaderCold, wUtils::IntDivCeil(minCapacity, componentPageSize_v<T>));
                }
            }
            else
            {
                const wIndex componentListHeaderIndex = sceneDenseIndex * componentSetup.slotListCount + componentListIndex_v<T>;
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
        [[nodiscard]] ComponentHandle<T> CreateComponent(SceneIndex sceneIndex)
        {
            const SceneIndex sceneDenseIndex = m_sceneSlotToDense[sceneIndex - sceneIndexStart];
            if constexpr (componentPageSize_v<T>)
            {
                const wIndex pageListHeaderIndex = sceneDenseIndex * componentSetup.pageListCount + componentListIndex_v<T>;
                // TODO:

                return ComponentHandle<T>();
            }
            else
            {
                const wIndex componentListHeaderIndex = sceneDenseIndex * componentSetup.slotListCount + componentListIndex_v<T>;
                ComponentListHeaderCold& componentListHeaderCold = m_componentListsCold[componentListHeaderIndex];
                if (componentListHeaderCold.freeList.Empty())
                {
                    if (componentListHeaderCold.slotCount == componentListHeaderCold.capacity)
                    {
                        ReallocateComponents<T>(m_componentListsHot[componentListHeaderIndex], componentListHeaderCold, wIndex newCapacity)
                    }
                    return ComponentHandle<T>();
                }
                
                return ComponentHandle<T>();
            }
        }

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
        [[nodiscard]] T& GetComponent(SceneIndex sceneIndex, ComponentIndex componentIndex) noexcept;
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

        struct ComponentListHeaderCold
        {
            wIndex slotCount;
            wIndex slotGenerationCount;
            wIndex denseCount;
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
        static void ReallocatePages(void*& data, PageListHeaderCold& headerCold, wIndex newPageCount)
        {
            std::size_t offset = newPageCount * sizeof(T*);
            
            offset = wUtils::AlignUp(offset, alignof(ComponentGeneration));
            const std::size_t componentGenerationOffset = offset;
            offset += componentPageSize_v<T> * sizeof(ComponentGeneration) * newPageCount;

            constexpr std::size_t alignment = wUtils::Max(alignof(T*), alignof(ComponentGeneration));

            std::byte* newMemory = static_cast<std::byte*>(
                ::operator new(offset, std::align_val_t(alignment))
            );

            if (data)
            {

                std::memcpy(newMemory, data, headerCold.pageCount * sizeof(T*));
                ::operator delete(data, std::align_val_t(alignof(T*)));

                if (headerCold.slotGenerationCount)
                {
                    std::memcpy(newMemory + componentGenerationOffset, data + componentGenerationOffset, headerCold.slotGenerationCount * sizeof(ComponentGeneration));
                }
                
            }
            for (wIndex pageIndex = headerCold.pageCount; pageIndex < newPageCount; ++pageIndex)
            {
                newMemory[pageIndex] = static_cast<T*>(
                    ::operator new(componentPageSize_v<T> * sizeof(T), std::align_val_t(alignof(T)))
                );
            }

            data = newMemory;

            headerCold.pageCount = newPageCount;
        }

        template<typename T>
        static void ReallocateComponents(void*& data, ComponentListHeaderCold& headerCold, wIndex newCapacity)
        {
            std::size_t offset = newCapacity * sizeof(T);

            offset = wUtils::AlignUp(offset, alignof(ComponentIndex));
            const std::size_t slotToDenseOffset = offset;
            offset += newCapacity * sizeof(ComponentIndex);

            offset = wUtils::AlignUp(offset, alignof(ComponentGeneration));
            const std::size_t componentGenerationOffset = offset;
            offset += newCapacity * sizeof(ComponentGeneration);

            constexpr std::size_t alignment = wUtils::maxAlignOf_v<T, ComponentIndex, ComponentGeneration>;

            if (data)
            {
                std::byte* newMemory = static_cast<std::byte*>(
                    ::operator new(offset, std::align_val_t(alignment))
                );

                if constexpr (std::is_trivially_copyable_v<T>)
                {
                    if (headerCold.denseCount)
                    {
                        std::memcpy(newMemory, data, headerCold.denseCount * sizeof(T));
                    }
                }
                else
                {
                    T* const begin = static_cast<T*>(data);
                    T* const end = begin + headerCold.denseCount;
                    T* dst = reinterpret_cast<T*>(newMemory);
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
                    std::memcpy(newMemory + slotToDenseOffset, static_cast<std::byte*>(data) + slotToDenseOffset, headerCold.slotCount * sizeof(ComponentIndex));
                }
                if (headerCold.slotGenerationCount)
                {
                    std::memcpy(newMemory + componentGenerationOffset, static_cast<std::byte*>(data) + componentGenerationOffset, headerCold.slotGenerationCount * sizeof(ComponentGeneration));
                }

                ::operator delete(data, std::align_val_t(alignment));

                data = newMemory;
            }
            else
            {
                data = ::operator new(offset, std::align_val_t(alignment));
            }
            
            headerCold.capacity = newCapacity;
        }
        
        Application& m_app;

        std::byte* m_scenes;
        void** m_componentListsHot;
        void** m_pageListsHot;
        SceneIndex* m_sceneSlotToDense;
        SceneGeneration* m_sceneGenerations;
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