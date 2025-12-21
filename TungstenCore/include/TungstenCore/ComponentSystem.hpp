#ifndef TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP
#define TUNGSTEN_CORE_COMPONENT_SYSTEM_HPP

namespace wCore
{
    class Application;

    class ComponentSystem
    {
    public:
        ComponentSystem(Application& app) noexcept;

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

        Application& m_app;
    };
}

#endif