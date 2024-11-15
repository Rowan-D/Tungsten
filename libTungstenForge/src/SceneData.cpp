#include "wForgePCH.hpp"
#include "SceneData.hpp"

namespace wForge {
    SceneData::SceneData()
        : m_components(),
        m_transforms(),
        m_meshRenderers(),
        m_scripts(),
        m_names()
    {

    }

    struct IntermediateData
    {
        struct ComponentTocken
        {
            ComponentTocken(uint32_t tabulation, std::string_view typeName, std::string_view name)
                : tabulation(tabulation), typeName(typeName), name(name)
            {
            }
            uint32_t tabulation;
            std::string_view typeName;
            std::string_view name;
        };

        IntermediateData()
            : componentTockens()
        {
        }

        std::vector<ComponentTocken> componentTockens;
    };

    static bool IsValidTypeName(const std::string_view& typeName)
    {
        if (std::isalpha(typeName[0]))
        {
            return false;
        }

        for (size_t i = 1; i < typeName.length(); ++i)
        return {};
    }

    bool SceneData::LoadSceneFromString(const std::string_view& sceneStr, ErrorList& errorList)
    {
        IntermediateData intData;

        size_t i = 0;
        while (i < sceneStr.size())
        {
            uint32_t tabulation;
            while (i < sceneStr.size())
            {
                char ch = sceneStr[i];
                if (ch == ' ' || ch == '\t')
                {
                    ++tabulation;
                }
                else if (std::isalpha(ch))
                {
                    break;
                }
                else if (ch == '\n')
                {
                    tabulation = 0;
                }
                ++i;
            }
            std::optional<std::string_view> typeName = 
            W_LOG_INFO("{}", tabulation);
            ++i;
        }

        /*std::vector<std::string_view> strings;
	    size_t i = 0;
        while (i < sceneStr.size())
        {
            char ch = sceneStr[i];
            if (isalpha(ch))
            {
                std::optional<std::string_view> name = GetTypeName(i, sceneStr, errorList);
                if (!name)
                {
                    return false;
                }
                strings.push_back(name.value());
            }
            else if (ch == '"' || ch == '\'')
            {
                std::optional<std::string_view> string = GetString(i, sceneStr, errorList);
                if (!string)
                {
                    return false;
                }
                strings.push_back(string.value());
            }
            ++i;
        }
        for (std::string_view str : strings)
        {
            W_LOG_DEBUG("{}", str);
        }
        return true;*/
    }
}
