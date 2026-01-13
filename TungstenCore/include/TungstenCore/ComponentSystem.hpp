#ifndef TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP
#define TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP

#include "ComponentSetup.hpp"
#include "TungstenCore/ComponentList.hpp"
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

    using SceneGeneration = ComponentGeneration;
    
    class SceneHandle
    {
    public:
        [[nodiscard]] inline SceneIndex GetSceneIndex() { return m_handle.index; }
        [[nodiscard]] inline SceneGeneration GetSceneGeneration() { return m_handle.generation; }

    private:
        SceneHandle(ComponentListHandle<SceneIndex> handle)
            : m_handle(handle) {}

        ComponentListHandle<SceneIndex> m_handle;
        friend class ComponentSystem;
    };

    template<typename T>
    class ComponentHandle
    {
    public:
        [[nodiscard]] inline SceneHandle& GetSceneHandle() { return sceneHandle; }
        [[nodiscard]] inline ComponentIndex GetComponentIndex() { return handle.index; }
        [[nodiscard]] inline ComponentGeneration GetComponentGeneration() { return handle.generation; }

    private:
        SceneHandle sceneHandle;
        ComponentListHandle<ComponentIndex> handle;
        friend class ComponentSystem;
    };

    class ComponentSystem
    {
    public:
        ComponentSystem(Application& app) noexcept;
        ~ComponentSystem() noexcept;

        inline void ReserveScenes(wIndex minCapacity) { m_sceneList.Reserve<SceneListSchema>(m_sceneMemory, minCapacity); }
        inline void ReserveSceneFreeList(wIndex minCapacity) { m_sceneList.freeList.Reserve(minCapacity); }
        [[nodiscard]] inline SceneHandle CreateScene() { return SceneHandle(m_sceneList.Create<SceneListSchema>(m_sceneMemory)); }
        [[nodiscard]] inline bool SceneExists(SceneHandle sceneHandle) const noexcept { return m_sceneList.Exists<SceneListSchema>(m_sceneMemory, sceneHandle.m_handle); }
        void DestroyScene(SceneIndex sceneIndex) noexcept;
        void DestroyScene(SceneHandle sceneHandle) noexcept;

        [[nodiscard]] inline wIndex GetSceneCount() const noexcept { return m_sceneList.Count(); }
        [[nodiscard]] inline wIndex GetSceneCapacity() const noexcept { return m_sceneList.Capacity<SceneListSchema>(); }
        [[nodiscard]] inline wIndex GetSceneFreeListCount() const noexcept { return m_sceneList.freeList.Count(); }
        [[nodiscard]] inline wIndex GetSceneFreeListCapacity() const noexcept { return m_sceneList.freeList.Capacity(); }
        [[nodiscard]] inline wIndex GetSceneSlotCount() const noexcept { return m_sceneList.slotCount; }

        template<typename T>
        void ReserveComponents(SceneIndex sceneIndex, wIndex minCapacity)
        {
            const SceneIndex componentListIndex = m_sceneList.GetDense<SceneListSchema, SceneIndex>(m_sceneMemory, sceneIndex - 1) * componentListCount_v<ComponentKindTag<T>> + componentListIndex_v<T>;
            ComponentListHeader<ComponentListLayout<ComponentKindTag<T>>, ComponentIndex>& componentListHeader = m_sceneList.GetDense<SceneListSchema, ComponentListHeader<ComponentListLayout<ComponentKindTag<T>>, ComponentIndex>>(m_sceneMemory)[componentListIndex];
            if (minCapacity > componentListHeader.template Capacity<SceneListSchema>())
            {
            //    componentListHeader.template IncreaseCapacity<ComponentListSchema<T>>(m_sceneList.Get<SceneListSchema, ComponentBuffer<ComponentListLayout<ComponentKindTag<T>>>>(m_sceneMemory)[sceneDenseIndex], minCapacity);
            }

            //const wIndex componentListIndex = m_sceneList.Get<SceneListSchema, ComponentIndex>(sceneDenseIndex)[sceneIndex - 1] * componentSetup.pageListCount + componentListIndex_v<T>;


            //ComponentListHeader<ComponentListLayout<ComponentKindTag<T>>, ComponentIndex>& componentListHeader = m_sceneList.Get<SceneListSchema, ComponentListHeader<ComponentListLayout<ComponentKindTag<T>>, ComponentIndex>>(m_sceneMemory)[sceneDenseIndex];
            //if (minCapacity > componentListHeader.template Capacity<SceneListSchema>())
            //{
            //    componentListHeader.template IncreaseCapacity<ComponentListSchema<T>>(m_sceneList.Get<SceneListSchema, ComponentBuffer<ComponentListLayout<ComponentKindTag<T>>>>(m_sceneMemory)[sceneDenseIndex], minCapacity);
            //}
        }

        template<typename T>
        bool TryReserveComponents(SceneHandle sceneHandle, wIndex minCapacity);

        template<typename T>
        void ReserveComponentFreeList(SceneIndex sceneIndex, wIndex minCapacity);
        template<typename T>
        bool TryReserveComponentFreeList(SceneHandle sceneHandle, wIndex minCapacity);

        /*template<typename T>
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
        }*/

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
        struct SceneData
        {
            wIndex nameIndex;
        };

        void ReallocateScenes(wIndex newCapacity);

        /*template<typename T>
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
        }*/
        
        Application& m_app;

        template<typename KindTag>
        using ComponentListLayout = ComponentLayout<KindTag, 1>;

        template<typename T>
        struct ComponentStoragePolicy
        {
            template<typename List>
            static inline void Relocate(const ComponentBuffer<ComponentListLayout<T>> data, std::byte* newMemory, std::size_t* offsets, wIndex denseCount) noexcept
            {
            }
        };

        template<typename T>
        using ComponentListSchema = ComponentSchema<
            ComponentTypeList<
                TypeCount<T, 1>
            >,
            ComponentTypeList<>,
            ComponentStoragePolicy<T>,
            componentPageSize_v<T>
        >;

        using SceneListLayout = ComponentLayout<SlotListTag, 5>;

        struct SceneStoragePolicy
        {
            template<typename List>
            static inline void Construct(ComponentBuffer<SceneListLayout>& data, wIndex denseIndex)
            {
                const std::size_t sceneStartComponentIndex = denseIndex * componentSetup.slotListCount;
                const std::size_t sceneStartPageIndex = denseIndex * componentSetup.pageListCount;

                std::memset(data.Get<List, ComponentBuffer<ComponentListLayout<SlotListTag>>>() + sceneStartComponentIndex, 0, sizeof(ComponentBuffer<ComponentListLayout<SlotListTag>>) * componentSetup.slotListCount);
                std::memset(data.Get<List, ComponentBuffer<ComponentListLayout<PageListTag>>>() + sceneStartPageIndex, 0, sizeof(ComponentBuffer<ComponentListLayout<PageListTag>>) * componentSetup.pageListCount);
                std::memset(data.Get<List, ComponentListHeader<ComponentListLayout<SlotListTag>, ComponentIndex>>() + sceneStartComponentIndex, 0, sizeof(ComponentListHeader<ComponentListLayout<SlotListTag>, ComponentIndex>) * componentSetup.slotListCount);
                std::memset(data.Get<List, ComponentListHeader<ComponentListLayout<PageListTag>, ComponentIndex>>() + sceneStartPageIndex, 0, sizeof(ComponentListHeader<ComponentListLayout<PageListTag>, ComponentIndex>) * componentSetup.pageListCount);
                std::construct_at(data.Get<List, SceneData>() + denseIndex);
            }

            template<typename List>
            static inline void Relocate(const ComponentBuffer<SceneListLayout>& data, std::byte* newMemory, std::size_t* offsets, wIndex denseCount) noexcept
            {
                std::memcpy(newMemory + GetOffset<List, ComponentBuffer<ComponentListLayout<SlotListTag>>>(offsets), data.Get<List, ComponentBuffer<ComponentListLayout<SlotListTag>>>(), denseCount * componentSetup.slotListCount * sizeof(ComponentBuffer<ComponentListLayout<SlotListTag>>));
                std::memcpy(newMemory + GetOffset<List, ComponentBuffer<ComponentListLayout<SlotListTag>>>(offsets), data.Get<List, ComponentBuffer<ComponentListLayout<PageListTag>>>(), denseCount * componentSetup.pageListCount * sizeof(ComponentBuffer<ComponentListLayout<PageListTag>>));
                std::memcpy(newMemory + GetOffset<List, ComponentListHeader<ComponentListLayout<SlotListTag>, ComponentIndex>>(offsets), data.Get<List, ComponentListHeader<ComponentListLayout<SlotListTag>, ComponentIndex>>(), denseCount * componentSetup.slotListCount * sizeof(ComponentListHeader<ComponentListLayout<SlotListTag>, ComponentIndex>));
                std::memcpy(newMemory + GetOffset<List, ComponentListHeader<ComponentListLayout<SlotListTag>, ComponentIndex>>(offsets), data.Get<List, ComponentListHeader<ComponentListLayout<PageListTag>, ComponentIndex>>(), denseCount * componentSetup.pageListCount * sizeof(ComponentListHeader<ComponentListLayout<PageListTag>, ComponentIndex>));
                std::memcpy(newMemory + GetOffset<List, SceneData>(offsets), data.Get<List, SceneData>(), denseCount * sizeof(SceneData));
            }

            static constexpr wIndex InitialCapacity = 8;
            static inline constexpr wIndex CalculateNextCapacity(wIndex current) noexcept
            {
                if (current)
                {
                    return current * 2;
                }
                return InitialCapacity;
            }
        };

        using SceneListSchema = ComponentSchema<
            ComponentTypeList<
                TypeCount<ComponentBuffer<ComponentListLayout<SlotListTag>>, componentListCount_v<SlotListTag>>,
                TypeCount<ComponentBuffer<ComponentListLayout<PageListTag>>, componentListCount_v<PageListTag>>
            >,
            ComponentTypeList<
                TypeCount<ComponentListHeader<ComponentListLayout<SlotListTag>, ComponentIndex>, componentListCount_v<SlotListTag>>,
                TypeCount<ComponentListHeader<ComponentListLayout<PageListTag>, ComponentIndex>, componentListCount_v<PageListTag>>,
                TypeCount<SceneData, 1>
            >,
            SceneStoragePolicy
        >;
        ComponentBuffer<SceneListLayout> m_sceneMemory;
        ComponentListHeader<SceneListLayout, SceneIndex> m_sceneList;
    };
}

#endif