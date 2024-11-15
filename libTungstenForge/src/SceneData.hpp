#pragma once
#include "ErrorList.hpp"

namespace wForge
{
    class SceneData
    {
        struct Transform
        {
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;
        };

        struct MeshRenderer
        {
            uint32_t meshIndex;
            uint32_t materialIndex;
        };

        struct Component
        {
            uint32_t nameIndex;
            uint32_t componentType;
            uint32_t componentIndex;
            uint32_t childeCount;
        };

        struct Script
        {
            uint32_t nameIndex;
        };

    public:
        SceneData();

        bool LoadSceneFromString(const std::string_view& sceneStr, ErrorList& errorList);

        static bool IsValidTypeName(const std::string_view& typeName);

    private:
        std::vector<Component> m_components;
        std::vector<Transform> m_transforms;
        std::vector<MeshRenderer> m_meshRenderers;
        std::vector<Script> m_scripts;
        std::vector<std::string> m_names;
    };
}
