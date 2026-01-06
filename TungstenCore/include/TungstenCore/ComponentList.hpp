#ifndef TUNGSTEN_CORE_COMPONENT_LIST_HPP
#define TUNGSTEN_CORE_COMPONENT_LIST_HPP

#include "TungstenUtils/wIndex.hpp"
#include <type_traits>

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
}

#endif