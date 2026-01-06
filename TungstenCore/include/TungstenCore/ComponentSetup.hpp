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

    template<typename T>
    inline constexpr wIndex componentListIndex_v = componentSetup.listIndexes[componentTypeIndexOf_v<T> - ComponentTypeIndexStart];

}

#endif