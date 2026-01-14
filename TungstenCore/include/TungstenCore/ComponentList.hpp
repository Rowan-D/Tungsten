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

    template<typename HotTypeList, typename ColdTypeList, typename StoragePolicyT, wIndex PageSize = 0>
    struct ComponentListSchema
    {
        using Hot = HotTypeList;
        using Cold = ColdTypeList;
        using StoragePolicy = StoragePolicyT;
        static constexpr wIndex pageSize = PageSize;
    };

    struct SlotListTag {};
    struct PageListTag {};

    template<typename Layout, wIndex N>
    struct ComponentListLayout;
    
    template<wIndex N>
    struct ComponentListLayout<SlotListTag, N>
    {
        using kind = SlotListTag;
        static constexpr wIndex bufferCount = N + 2;
    };

    template<wIndex N>
    struct ComponentListLayout<PageListTag, N>
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

    template<typename T>
    inline void AdvanceComponentListLayoutOffset(std::size_t& offset, wIndex newCapacity, std::size_t& outOffset) noexcept
    {
        offset = wUtils::AlignUp(offset, alignof(T));
        outOffset = offset;
        offset += newCapacity * sizeof(T);
    }

    template<typename... Ts, std::size_t... Is>
    inline void BuildComponentListLayoutImpl(std::size_t& offset, wIndex newCapacity, std::size_t* offsets, std::index_sequence<Is...>) noexcept { (AdvanceComponentListLayoutOffset<Ts>(offset, newCapacity, offsets[Is]), ...); }

    template<typename T0, typename... Ts>
    [[nodiscard]] inline std::size_t BuildComponentListLayoutFromTypes(wIndex newCapacity, std::size_t* offsets) noexcept
    {
        std::size_t offset = newCapacity * sizeof(T0);
        BuildComponentListLayoutImpl<Ts...>(offset, newCapacity, offsets, std::index_sequence_for<Ts...>{});
        return offset;
    }

    template<typename List>
    struct BuildComponentListLayoutFromList;

    template<typename T0, typename... Ts>
    struct BuildComponentListLayoutFromList<wCore::ComponentTypeList<T0, Ts...>>
    {
        [[nodiscard]] static inline std::size_t Run(wIndex newCapacity, std::size_t* offsets) noexcept { return BuildComponentListLayoutFromTypes<T0, Ts...>(newCapacity, offsets); }
    };

    template<typename List>
    [[nodiscard]] inline std::size_t BuildComponentListLayout(wIndex newCapacity, std::size_t* offsets) noexcept { return BuildComponentListLayoutFromList<List>::Run(newCapacity, offsets); }

    template<typename List, typename T>
    [[nodiscard]] static std::size_t ComponentListGetOffset(const std::size_t* offsets) noexcept
    {
        constexpr wIndex idx = IndexOf_v<T, List>;
        if constexpr (idx)
        {
            return offsets[idx - 1];
        }
        return 0;
    }
}

#endif