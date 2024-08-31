#pragma once
#include "core.hpp"

namespace wCore
{
    struct Transform
    {
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        glm::mat4 matrix;
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

    };

    class Scene
    {
    public:
        uint32_t AddComponent(uint32_t type, std::string path);
        uint32_t AddScene(Scene scene, std::string path);

    private:
        std::vector<Component> m_components;
        std::vector<Transform> m_transforms;
        std::vector<MeshRenderer> m_meshRenderers;
        std::vector<Script> m_scripts;
        std::vector<std::string> m_names;
    };
}

