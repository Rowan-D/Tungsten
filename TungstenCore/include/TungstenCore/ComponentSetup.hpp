#ifndef TUNGSTEN_CORE_COMPONENT_SETUP_HPP
#define TUNGSTEN_CORE_COMPONENT_SETUP_HPP

#include "generated/componentDeclarations.hpp"
#include <array>

namespace wCore
{
    using ComponentTypeIndex = wIndex;
    inline constexpr ComponentTypeIndex InvalidComponentType = 0;
    inline constexpr ComponentTypeIndex ComponentTypeIndexStart = 1;

    template<typename T>
    struct ComponentTypeIndexOf
    {
        static constexpr ComponentTypeIndex value = IndexOf_v<T, wProject::Generated::ComponentTypes> + ComponentTypeIndexStart;
    };

    template<typename T>
    inline constexpr ComponentTypeIndex componentTypeIndexOf_v = ComponentTypeIndexOf<T>::value;

    template<typename T>
    inline constexpr wIndex componentPageSize_v = wProject::Generated::componentPageSizes[componentTypeIndexOf_v<T> - ComponentTypeIndexStart];

    template<typename T>
    using ComponentKindTag = std::conditional_t<(componentPageSize_v<T> == 0), SlotListTag, PageListTag>;

    struct ComponentSetup
    {
        consteval ComponentSetup()
            : pageListCount(0), slotListCount(0), listIndexes{}
        {
            for (ComponentTypeIndex componentTypeIndex = ComponentTypeIndexStart; componentTypeIndex <= wProject::Generated::ComponentTypes::count; ++componentTypeIndex)
            {
                const wIndex pageSize = wProject::Generated::componentPageSizes[componentTypeIndex - ComponentTypeIndexStart];
                const bool isPaged = (pageSize != 0);
                listIndexes[componentTypeIndex - ComponentTypeIndexStart] = isPaged ? pageListCount++ : slotListCount++;
            }
        }

        wIndex pageListCount;
        wIndex slotListCount;
        std::array<wIndex, wProject::Generated::ComponentTypes::count> listIndexes;
    };

    inline constexpr ComponentSetup componentSetup{};

    template<class KindTag>
    inline constexpr wIndex componentListCount_v = 0;

    template<>
    inline constexpr wIndex componentListCount_v<SlotListTag> = componentSetup.slotListCount;

    template<>
    inline constexpr wIndex componentListCount_v<PageListTag> = componentSetup.pageListCount;

    template<typename T>
    inline constexpr wIndex componentListIndex_v = componentSetup.listIndexes[componentTypeIndexOf_v<T> - ComponentTypeIndexStart];

    template<typename Layout>
    struct ComponentBuffer
    {
        template<typename List, typename T>
        [[nodiscard]] inline T* Get() noexcept
        {
            constexpr wIndex idx = IndexOfTypeCount_v<T, List>;
            return static_cast<T*>(ptrs[idx]);
        }

        template<typename List, typename T>
        [[nodiscard]] inline const T* Get() const noexcept
        {
            constexpr wIndex idx = IndexOfTypeCount_v<T, List>;
            return static_cast<const T*>(ptrs[idx]);
        }

        void* ptrs[Layout::bufferCount]{};
    };

    template<typename Layout, std::integral IndexT>
    struct ComponentListHeader;

    template<typename Layout, std::integral IndexT>
        requires std::is_same_v<typename Layout::kind, SlotListTag>
    struct ComponentListHeader<Layout, IndexT>
    {
        template<typename Schema>
        using TypeList = Concat_t<typename Schema::Hot, ComponentTypeList<TypeCount<IndexT, 1>, TypeCount<ComponentGeneration, 1>>, typename Schema::Cold>;
        
        template<typename Schema>
        void Reserve(ComponentBuffer<Layout>& data, wIndex minCapacity)
        {
            if (minCapacity > capacity)
            {
                IncreaseCapacity<Schema>(data, minCapacity);
            }
        }

        template<typename Schema>
        [[nodiscard]] ComponentListHandle<IndexT> Create(ComponentBuffer<Layout>& data)
        {
            if (freeList.Empty())
            {
                if (slotCount == capacity)
                {
                    Reallocate<Schema>(data, Schema::StoragePolicy::CalculateNextCapacity(capacity));
                }
                Schema::StoragePolicy::template Construct<TypeList<Schema>>(data, denseCount);
                Data<Schema, IndexT>(data)[slotCount] = denseCount++;
                if (slotCount < slotGenerationCount)
                {
                    ++slotCount;
                    return ComponentListHandle<IndexT>(slotCount, Data<Schema, ComponentGeneration>(data)[slotCount - 1]);
                }
                else
                {
                    ++slotCount;
                    ++slotGenerationCount;
                    return ComponentListHandle<IndexT>(slotCount, 0);
                }
            }
            const IndexT index = freeList.Remove();
            Schema::StoragePolicy::template Construct<TypeList<Schema>>(data, denseCount);
            Data<Schema, IndexT>(data)[index - 1] = denseCount++;
            return ComponentListHandle<IndexT>(index, Data<Schema, ComponentGeneration>(data)[index - 1]);
        }

        template<typename Schema>
        [[nodiscard]] inline bool Exists(const ComponentBuffer<Layout> data, ComponentListHandle<IndexT> handle) const noexcept { return handle.index <= slotCount && handle.generation == Data<Schema, ComponentGeneration>(data)[handle.index - 1]; }

        template<typename Schema, typename T>
        [[nodiscard]] inline T& Get(ComponentBuffer<Layout>& data, IndexT index) noexcept
        {
            const IndexT denseIndex = GetDense<Schema, IndexT>(data, index);
            return Data<Schema, T>(data)[denseIndex];
        }

        template<typename Schema, typename T>
        [[nodiscard]] inline const T& Get(const ComponentBuffer<Layout>& data, IndexT index) const noexcept
        {
            const IndexT denseIndex = GetDense<Schema, IndexT>(data, index);
            return Data<Schema, T>(data)[denseIndex];
        }

        template<typename Schema, typename T>
        [[nodiscard]] inline T& GetDense(ComponentBuffer<Layout>& data, IndexT denseIndex) noexcept
        {
            return Data<Schema, T>(data)[denseIndex];
        }

        template<typename Schema, typename T>
        [[nodiscard]] inline const T& GetDense(const ComponentBuffer<Layout>& data, IndexT denseIndex) const noexcept
        {
            return Data<Schema, T>(data)[denseIndex];
        }

        template<typename Schema, typename T>
        [[nodiscard]] inline T* Data(ComponentBuffer<Layout>& data) noexcept
        {
            return data.template Get<TypeList<Schema>, T>();
        }

        template<typename Schema, typename T>
        [[nodiscard]] inline const T* Data(const ComponentBuffer<Layout>& data) const noexcept
        {
            return data.template Get<TypeList<Schema>, T>();
        }

        [[nodiscard]] inline wIndex Count() const noexcept { return slotCount - freeList.Count(); }

        template<typename Schema>
        [[nodiscard]] inline wIndex Capacity() const noexcept { return capacity; }
        
        template<typename Schema>
        inline void IncreaseCapacity(ComponentBuffer<Layout>& data, wIndex minCapacity) { Reallocate<Schema>(data, minCapacity); }

        template<typename Schema>
        void Reallocate(ComponentBuffer<Layout>& data, wIndex newCapacity)
        {
            capacity = newCapacity;

            std::size_t offsets[TypeList<Schema>::count - 1];
            const std::size_t offset = BuildTypeCountLayout<TypeList<Schema>>(newCapacity, offsets);

            constexpr std::size_t alignment = maxAlignOfTypeCounts_v<TypeList<Schema>>;
            if (data.ptrs[0])
            {
                std::byte* newMemory = static_cast<std::byte*>(
                    ::operator new(offset, std::align_val_t(alignment))
                );

                if (slotGenerationCount)
                {
                    std::memcpy(newMemory + GetOffset<TypeList<Schema>, ComponentGeneration>(offsets), Data<Schema, ComponentGeneration>(data), slotGenerationCount * sizeof(ComponentGeneration));
                    if (slotCount)
                    {
                        std::memcpy(newMemory + GetOffset<TypeList<Schema>, IndexT>(offsets), Data<Schema, IndexT>(data), slotCount * sizeof(IndexT));
                        if (denseCount)
                        {
                            Schema::StoragePolicy::template Relocate<TypeList<Schema>>(data, newMemory, offsets, denseCount);
                        }
                    }
                }
                
                ::operator delete(data.ptrs[0], std::align_val_t(alignment));

                data.ptrs[0] = newMemory;
            }
            else
            {
                data.ptrs[0] = ::operator new(offset, std::align_val_t(alignment));
            }

            for (wIndex i = 0; i < TypeList<Schema>::count - 1; ++i)
            {
                data.ptrs[i + 1] = static_cast<std::byte*>(data.ptrs[0]) + offsets[i];
            }
        }
        
        wIndex slotCount{};
        wIndex slotGenerationCount{};
        wIndex denseCount{};
        wIndex capacity{};
        wUtils::RelocatableFreeListHeader<IndexT> freeList{};
    };

    template<typename Layout, std::integral IndexT>
        requires std::is_same_v<typename Layout::kind, PageListTag>
    struct ComponentListHeader<Layout, IndexT>
    {
        template<typename Schema>
        using TypeList = Concat_t<typename Schema::Hot, ComponentTypeList<TypeCount<ComponentGeneration, 1>>, typename Schema::Cold>;

        [[nodiscard]] inline wIndex Count() const noexcept { return slotCount - freeList.Count(); }

        template<typename Schema>
        [[nodiscard]] inline wIndex Capacity() const noexcept { return pageCount * Schema::pageSize; }

        template<typename Schema>
        inline void IncreaseCapacity(ComponentBuffer<Layout>& data, wIndex minCapacity) { Reallocate(data, wUtils::IntDivCeil(minCapacity, Schema::pageSize)); }
        
        template<typename Schema>
        void Reallocate(ComponentBuffer<Layout>& data, wIndex newPageCount)
        {
        }

        wIndex slotCount{};
        wIndex slotGenerationCount{};
        wIndex pageCount{};
        wUtils::RelocatableFreeListHeader<IndexT> freeList{};
    };
}

#endif