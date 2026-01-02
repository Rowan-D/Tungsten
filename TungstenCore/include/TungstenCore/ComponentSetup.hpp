#ifndef TUNGSTEN_CORE_COMPONENT_SETUP_HPP
#define TUNGSTEN_CORE_COMPONENT_SETUP_HPP

#include "generated/componentDeclarations.hpp"

namespace wCore
{
    using ComponentTypeIndex = wIndex;

    template<typename T>
    struct ComponentTypeIndexOf
    {
        static constexpr ComponentTypeIndex value = IndexOf_v<T, wProject::Generated::ComponentTypes>;
    };

    template<typename T>
    ComponentTypeIndex ComponentTypeIndexOf_v = ComponentTypeIndexOf<T>::value;
}

#endif