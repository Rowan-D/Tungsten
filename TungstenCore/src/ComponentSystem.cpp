#include "wCorePCH.hpp"
#include "TungstenCore/ComponentSystem.hpp"

namespace wCore
{
    ComponentSystem::ComponentSystem(Application& app) noexcept
        : m_app(app)
    {
    }

    ComponentSystem::~ComponentSystem() noexcept
    {
    }
}