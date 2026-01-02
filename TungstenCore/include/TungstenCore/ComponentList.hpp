#ifndef TUNGSTEN_CORE_COMPONENT_LIST_HPP
#define TUNGSTEN_CORE_COMPONENT_LIST_HPP

#include "TungstenUtils/wIndex.hpp"

namespace wCore
{
    template<typename... Ts>
    struct ComponentList
    {
        static constexpr wIndex count = sizeof...(Ts);
    };

    template<typename T, typename List>
    struct IndexOf;

    template<typename T, typename... Ts>
    struct IndexOf<T, ComponentList<T, Ts...>>
    {
        static constexpr wIndex value = 0;
    };

    template<typename T, typename U, typename... Ts>
    struct IndexOf<T, ComponentList<U, Ts...>>
    {
        static constexpr wIndex value = 1 + IndexOf<T, ComponentList<Ts...>>::value;
    };

    template<typename T>
    struct IndexOf<T, ComponentList<>>
    {
        static_assert(!std::is_same_v<T, T>, "Type not found in ComponentList");
    };

    template<typename T, typename List>
    inline constexpr wIndex IndexOf_v = IndexOf<T, List>::value;
}

#endif