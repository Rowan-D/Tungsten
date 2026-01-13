#ifndef TUNGSTEN_CORE_COMPONENT_LIST_HPP
#define TUNGSTEN_CORE_COMPONENT_LIST_HPP

#include "TungstenUtils/wIndex.hpp"
#include <type_traits>
#include <utility>
#include "TungstenUtils/alignment.hpp"
#include "TungstenUtils/RelocatableFreeListHeader.hpp"

namespace wCore
{
    template<typename... Ts>
    struct ComponentTypeList
    {
        static constexpr wIndex count = sizeof...(Ts);
    };

    template<typename>
    inline constexpr bool dependent_false_v = false;

    template<typename T, typename List>
    struct IndexOf;

    template<typename T, typename... Ts>
    struct IndexOf<T, ComponentTypeList<T, Ts...>>
    {
        static constexpr wIndex value = 0;
    };

    template<typename T, typename U, typename... Ts>
    struct IndexOf<T, ComponentTypeList<U, Ts...>>
    {
        static constexpr wIndex value = 1 + IndexOf<T, ComponentTypeList<Ts...>>::value;
    };

    template<typename T>
    struct IndexOf<T, ComponentTypeList<>>
    {
        static_assert(dependent_false_v<T>, "Type not found in ComponentTypeList");
    };

    template<typename T, typename List>
    inline constexpr wIndex IndexOf_v = IndexOf<T, List>::value;

    template<typename... Lists>
    struct Concat;

    template<>
    struct Concat<> { using type = ComponentTypeList<>; };

    template<typename... Ts>
    struct Concat<ComponentTypeList<Ts...>> { using type = ComponentTypeList<Ts...>; };

    template<typename... Ts, typename... Us, typename... Rest>
    struct Concat<ComponentTypeList<Ts...>, ComponentTypeList<Us...>, Rest...>
    {
        using type = typename Concat<ComponentTypeList<Ts..., Us...>, Rest...>::type;
    };

    template<typename... Lists>
    using Concat_t = typename Concat<Lists...>::type;

    template<typename T, wIndex N>
    struct TypeCount
    {
        using type = T;
        static constexpr wIndex count = N;
    };

    template<typename T, typename List>
    struct IndexOfTypeCount;

    template<typename T, wIndex N, typename... TCs>
    struct IndexOfTypeCount<T, ComponentTypeList<TypeCount<T, N>, TCs...>>
    {
        static constexpr wIndex value = 0;
    };

    template<typename T, typename TC0, typename... TCs>
    struct IndexOfTypeCount<T, ComponentTypeList<TC0, TCs...>>
    {
        static constexpr wIndex value = 1 + IndexOfTypeCount<T, ComponentTypeList<TCs...>>::value;
    };

    template<typename T>
    struct IndexOfTypeCount<T, ComponentTypeList<>>
    {
        static_assert(dependent_false_v<T>, "Type not found in ComponentTypeList<TypeCount<...>>");
    };

    template<typename T, typename List>
    inline constexpr wIndex IndexOfTypeCount_v = IndexOfTypeCount<T, List>::value;

    template<typename HotTypeList, typename ColdTypeList, typename StoragePolicyT, wIndex PageSize = 0>
    struct ComponentSchema
    {
        using Hot = HotTypeList;
        using Cold = ColdTypeList;
        using StoragePolicy = StoragePolicyT;
        static constexpr wIndex pageSize = PageSize;
    };

    struct SlotListTag {};
    struct PageListTag {};

    template<typename Layout, wIndex N>
    struct ComponentLayout;
    
    template<wIndex N>
    struct ComponentLayout<SlotListTag, N>
    {
        using kind = SlotListTag;
        static constexpr wIndex bufferCount = N + 2;
    };

    template<wIndex N>
    struct ComponentLayout<PageListTag, N>
    {
        using kind = PageListTag;
        static constexpr wIndex bufferCount = N + 1;
    };

    using ComponentGeneration = uint64_t;

    template<std::integral IndexT>
    struct ComponentListHandle
    {
        ComponentListHandle(IndexT a_index, ComponentGeneration a_generation)
            : index(a_index), generation(a_generation) {}

        IndexT index;
        ComponentGeneration generation;
    };

    template<typename List>
    inline constexpr std::size_t maxAlignOfTypeCounts_v = 1;

    template<>
    inline constexpr std::size_t maxAlignOfTypeCounts_v<ComponentTypeList<>> = 1;
    
    template<typename TC0, typename... TCs>
    inline constexpr std::size_t maxAlignOfTypeCounts_v<ComponentTypeList<TC0, TCs...>> = std::max({ alignof(std::remove_cvref_t<typename TC0::type>), alignof(std::remove_cvref_t<typename TCs::type>)... });
    
    template<typename TC>
    inline void AdvanceOffsetForTypeCount(std::size_t& offset, wIndex newCapacity, std::size_t& outOffset) noexcept
    {
        using T = typename TC::type;
        constexpr wIndex N = TC::count;

        offset = wUtils::AlignUp(offset, alignof(T));
        outOffset = offset;
        offset += newCapacity * N * sizeof(T);
    }

    template<typename... TCs, std::size_t... Is>
    inline void BuildTypeCountLayoutImpl(std::size_t& offset, wIndex newCapacity, std::size_t* offsets, std::index_sequence<Is...>) noexcept { (AdvanceOffsetForTypeCount<TCs>(offset, newCapacity, offsets[Is]), ...); }

    template<typename TC>
    concept TypeCountLike = requires
    {
        typename TC::type;
        {
            TC::count
        }
        -> std::convertible_to<wIndex>;
    };

    template<TypeCountLike TC0, TypeCountLike... TCs>
    [[nodiscard]] inline std::size_t BuildTypeCountLayout(wIndex newCapacity, std::size_t* offsets) noexcept
    {
        using T0 = typename TC0::type;
        constexpr wIndex N0 = TC0::count;

        std::size_t offset = newCapacity * N0 * sizeof(T0);
        BuildTypeCountLayoutImpl<TCs...>(offset, newCapacity, offsets, std::index_sequence_for<TCs...>{});
        return offset;
    }

    template<typename List>
    struct BuildTypeCountLayoutFromList;

    template<typename TC0, typename... TCs>
    struct BuildTypeCountLayoutFromList<wCore::ComponentTypeList<TC0, TCs...>>
    {
        static std::size_t Run(wIndex newCapacity, std::size_t* offsets) noexcept
        {
            return BuildTypeCountLayout<TC0, TCs...>(newCapacity, offsets);
        }
    };

    template<typename List>
    [[nodiscard]] inline std::size_t BuildTypeCountLayout(wIndex newCapacity, std::size_t* offsets) noexcept
    {
        return BuildTypeCountLayoutFromList<List>::Run(newCapacity, offsets);
    }

    template<typename List, typename T>
    [[nodiscard]] static std::size_t GetOffset(const std::size_t* offsets) noexcept
    {
        constexpr wIndex idx = IndexOfTypeCount_v<T, List>;
        if constexpr (idx)
        {
            return offsets[idx - 1];
        }
        return 0;
    }
}

#endif