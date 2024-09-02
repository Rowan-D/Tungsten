#include "wForgePCH.hpp"
#include "SceneData.hpp"

namespace wForge
{
    SceneData::SceneData()
        : m_components(),
        m_transforms(),
        m_meshRenderers(),
        m_scripts(),
        m_names()
    {

    }

    bool SceneData::LoadSceneFromString(const std::string& sceneStr, ErrorList& errorList)
    {

        return true;
    }

    bool SceneData::IsValidTypeName(const std::string_view& typeName)
    {
        if (std::isalpha(typeName[0]))
        {
            return false;
        }

        for (size_t i = 1; i < typeName.length(); ++i)
        {
            char ch = typeName[i];
            if (!(std::isalpha(ch) || std::isdigit(ch) || ch == '_'))
            {
                return false;
            }
        }

        return true;
    }
}