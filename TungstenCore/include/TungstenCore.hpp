#ifndef TUNGSTEN_CORE_HPP
#define TUNGSTEN_CORE_HPP

#include <vector>
#include "TungstenUtils.hpp"

namespace wCore
{
    class ComponentManager
    {
    public:

    private:
        std::vector<Component> m_components;
        struct Component
        {
            uint32_t type;
            uint32_t index;
        };
    };

    template<typename T>
    class ComponentList
    {
    public:
        ComponentList(ComponentManager& componentManager)
        {
        }

    private:
    };
}

#endif