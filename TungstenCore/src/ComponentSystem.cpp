#include "wCorePCH.hpp"
#include "TungstenCore/ComponentSystem.hpp"

#include "TungstenCore/ComponentSetup.hpp"

namespace wCore
{
    ComponentSystem::ComponentSystem(Application& app) noexcept
        : m_app(app),
        m_sceneCapacity(0)
    {
    }

    ComponentSystem::~ComponentSystem() noexcept
    {
    }

    void ComponentSystem::ReserveScenes(wIndex minCapacity)
    {
        if (minCapacity > m_sceneCapacity)
        {
            ReallocateScenes(minCapacity);
        }
    }

    void ComponentSystem::ReallocateScenes(wIndex newCapacity)
    {
        m_sceneCapacity = newCapacity;

        /*std::size_t offset = 0;

        offset = wUtils::AlignUp(offset, alignof(ComponentListHeaderHot));
        const std::size_t componentListHotOffset = offset;
        offset += wProject::Generated::ComponentTypes::count * sizeof(ComponentListHeaderHot) * newCapacity;

        offset = wUtils::AlignUp(offset, alignof(PageListHeaderHot));
        const std::size_t pageListHotOffset = offset;
        offset += m_createCtx.GetCurrentPageListCount() * newCapacity * sizeof(PageListHeaderHot);

        offset = wUtils::AlignUp(offset, alignof(SceneGeneration));
        const std::size_t sceneGenerationOffset = offset;
        offset += newCapacity * sizeof(SceneGeneration);

        offset = wUtils::AlignUp(offset, alignof(ComponentListHeaderCold));
        const std::size_t componentListColdOffset = offset;
        offset += m_createCtx.GetCurrentComponentListCount() * newCapacity * sizeof(ComponentListHeaderCold);

        offset = wUtils::AlignUp(offset, alignof(PageListHeaderCold));
        const std::size_t pageListColdOffset = offset;
        offset += m_createCtx.GetCurrentPageListCount() * newCapacity * sizeof(PageListHeaderCold);

        offset = wUtils::AlignUp(offset, alignof(SceneData));
        const std::size_t sceneDataOffset = offset;
        offset += newCapacity * sizeof(SceneData);

        constexpr std::size_t alignment = wUtils::MaxAlignOf<ComponentListHeaderHot, PageListHeaderHot, SceneGeneration, ComponentListHeaderCold, PageListHeaderCold, SceneData>;
        if (m_scenes)
        {
            std::byte* newScenes = static_cast<std::byte*>(
                ::operator new(offset, std::align_val_t(alignment))
            );

            ComponentListHeaderHot* newComponentListsHot = reinterpret_cast<ComponentListHeaderHot*>(newScenes + componentListHotOffset);
            PageListHeaderHot* newPageListsHot = reinterpret_cast<PageListHeaderHot*>(newScenes + pageListHotOffset);
            SceneGeneration* newSceneGenerations = reinterpret_cast<SceneGeneration*>(newScenes + sceneGenerationOffset);
            ComponentListHeaderCold* newComponentListsCold = reinterpret_cast<ComponentListHeaderCold*>(newScenes + componentListColdOffset);
            PageListHeaderCold* newPageListsCold = reinterpret_cast<PageListHeaderCold*>(newScenes + pageListColdOffset);
            SceneData* newSceneData = reinterpret_cast<SceneData*>(newScenes + sceneDataOffset);

            if (m_sceneSlotCount)
            {
                std::memcpy(newComponentListsHot, m_createCtx.componentListsHot, m_sceneSlotCount * m_createCtx.GetCurrentComponentListCount() * sizeof(ComponentListHeaderHot));
                std::memcpy(newPageListsHot, m_createCtx.pageListsHot, m_sceneSlotCount * m_createCtx.GetCurrentPageListCount() * sizeof(PageListHeaderHot));
                std::memcpy(newSceneGenerations, m_sceneGenerations, m_sceneSlotCount * sizeof(SceneGeneration));
                std::memcpy(newComponentListsCold, m_createCtx.componentListsCold, m_sceneSlotCount * m_createCtx.GetCurrentComponentListCount() * sizeof(ComponentListHeaderCold));
                std::memcpy(newPageListsCold, m_createCtx.pageListsCold, m_sceneSlotCount * m_createCtx.GetCurrentPageListCount() * sizeof(ComponentListHeaderCold));
                std::memcpy(newSceneData, m_sceneData, m_sceneSlotCount * sizeof(SceneData));
            }

            ::operator delete(m_scenes, std::align_val_t(alignment));

            m_scenes = newScenes;
        }
        else
        {
            m_scenes = static_cast<std::byte*>(
                ::operator new(offset, std::align_val_t(alignment))
            );
        }

        m_createCtx.componentListsHot = reinterpret_cast<ComponentListHeaderHot*>(m_scenes + componentListHotOffset);
        m_createCtx.pageListsHot = reinterpret_cast<PageListHeaderHot*>(m_scenes + pageListHotOffset);
        m_sceneGenerations = reinterpret_cast<SceneGeneration*>(m_scenes + sceneGenerationOffset);
        m_createCtx.componentListsCold = reinterpret_cast<ComponentListHeaderCold*>(m_scenes + componentListColdOffset);
        m_createCtx.pageListsCold = reinterpret_cast<PageListHeaderCold*>(m_scenes + pageListColdOffset);
        m_sceneData = reinterpret_cast<SceneData*>(m_scenes + sceneDataOffset);*/
    }
}