#include "wCorePCH.hpp"
#include "TungstenCore/Application.hpp"

namespace wCore
{
    Application::Application()
        : m_componentSystem(*this)
    {
    }

    Application::RunOutput Application::Run()
    {
        W_DEBUG_LOG_INFO("Hello, From Application.Run!");
        return Application::RunOutput(0);
    }
}