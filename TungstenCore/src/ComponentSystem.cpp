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

    SceneHandle ComponentSystem::CreateScene()
    {
        if (m_sceneFreeList.Empty())
        {
            if (m_sceneSlotCount == m_sceneCapacity)
            {
                ReallocateScenes(CalculateNextCapacity(m_sceneCapacity));
            }

            const std::size_t sceneStartComponentIndex = m_sceneDenseCount * componentSetup.slotListCount;
            const std::size_t sceneStartPageIndex = m_sceneDenseCount * componentSetup.pageListCount;

            std::construct_at(m_sceneSlots + m_sceneSlotCount, m_sceneDenseCount++);
            std::memset(m_componentListsHot + sceneStartComponentIndex, 0, sizeof(ComponentListHeaderHot) * componentSetup.slotListCount);
            std::memset(m_pageListsHot + sceneStartPageIndex, 0, sizeof(PageListHeaderHot) * componentSetup.pageListCount);
            std::memset(m_componentListsCold + sceneStartComponentIndex, 0, sizeof(ComponentListHeaderCold) * componentSetup.slotListCount);
            std::memset(m_pageListsCold + sceneStartPageIndex, 0, sizeof(PageListHeaderCold) * componentSetup.pageListCount);
            std::construct_at(m_sceneData + sceneSlot.denseIndex);

            ++m_sceneSlotCount;
            return SceneHandle(m_sceneSlotCount - 1, m_sceneSlots[m_sceneSlotCount - 1].generation);
        }
        const SceneIndex sceneIndex = m_sceneFreeList.Remove();

        SceneSlot& sceneSlot = m_sceneSlots[sceneIndex - SceneIndexStart];
        sceneSlot.denseIndex = m_sceneDenseCount++;
        ++sceneSlot.generation.generation;

        const std::size_t sceneStartComponentIndex = sceneSlot.denseIndex * componentSetup.slotListCount;
        const std::size_t sceneStartPageIndex = sceneSlot.denseIndex * componentSetup.pageListCount;

        std::memset(m_componentListsHot + sceneStartComponentIndex, 0, sizeof(ComponentListHeaderHot) * componentSetup.slotListCount);
        std::memset(m_pageListsHot + sceneStartPageIndex, 0, sizeof(PageListHeaderHot) * componentSetup.pageListCount);
        std::memset(m_componentListsCold + sceneStartComponentIndex, 0, sizeof(ComponentListHeaderCold) * componentSetup.slotListCount);
        std::memset(m_pageListsCold + sceneStartPageIndex, 0, sizeof(PageListHeaderCold) * componentSetup.pageListCount);
        std::construct_at(m_sceneData + sceneSlot.denseIndex);

        return SceneHandle(sceneIndex, m_sceneSlots[sceneIndex].generation);
    }

    /*void DestroyScene(SceneIndex sceneIndex) m_sceneGenerationsnoexcept
    {
    }

    void DestroyScene(SceneHandle sceneHandle) noexcept;
    {
    }*/

    void ComponentSystem::ReallocateScenes(wIndex newCapacity)
    {
        m_sceneCapacity = newCapacity;

        std::size_t offset = 0;

        offset = wUtils::AlignUp(offset, alignof(SceneSlot));
        const std::size_t sceneSlotOffset = offset;
        offset += newCapacity * sizeof(SceneSlot);

        offset = wUtils::AlignUp(offset, alignof(ComponentListHeaderHot));
        const std::size_t componentListHotOffset = offset;
        offset += componentSetup.slotListCount * sizeof(ComponentListHeaderHot) * newCapacity;

        offset = wUtils::AlignUp(offset, alignof(PageListHeaderHot));
        const std::size_t pageListHotOffset = offset;
        offset += componentSetup.pageListCount * newCapacity * sizeof(PageListHeaderHot);

        offset = wUtils::AlignUp(offset, alignof(ComponentListHeaderCold));
        const std::size_t componentListColdOffset = offset;
        offset += componentSetup.slotListCount * newCapacity * sizeof(ComponentListHeaderCold);

        offset = wUtils::AlignUp(offset, alignof(PageListHeaderCold));
        const std::size_t pageListColdOffset = offset;
        offset += componentSetup.pageListCount * newCapacity * sizeof(PageListHeaderCold);

        offset = wUtils::AlignUp(offset, alignof(SceneData));
        const std::size_t sceneDataOffset = offset;
        offset += newCapacity * sizeof(SceneData);

        constexpr std::size_t alignment = wUtils::MaxAlignOf<SceneSlot, ComponentListHeaderHot, PageListHeaderHot, ComponentListHeaderCold, PageListHeaderCold, SceneData>;
        if (m_scenes)
        {
            std::byte* newScenes = static_cast<std::byte*>(
                ::operator new(offset, std::align_val_t(alignment))
            );

            SceneSlot* newSceneSlot = reinterpret_cast<SceneSlot*>(newScenes + sceneSlotOffset);
            ComponentListHeaderHot* newComponentListsHot = reinterpret_cast<ComponentListHeaderHot*>(newScenes + componentListHotOffset);
            PageListHeaderHot* newPageListsHot = reinterpret_cast<PageListHeaderHot*>(newScenes + pageListHotOffset);
            ComponentListHeaderCold* newComponentListsCold = reinterpret_cast<ComponentListHeaderCold*>(newScenes + componentListColdOffset);
            PageListHeaderCold* newPageListsCold = reinterpret_cast<PageListHeaderCold*>(newScenes + pageListColdOffset);
            SceneData* newSceneData = reinterpret_cast<SceneData*>(newScenes + sceneDataOffset);

            if (m_sceneSlotGenerationsCount)
            {
                std::memcpy(newSceneData, m_sceneSlot, m_sceneSlotCount * sizeof(SceneSlot));
                if (m_sceneDenseCount)
                {
                    std::memcpy(newComponentListsHot, m_componentListsHot, m_sceneDenseCount * componentSetup.slotListCount * sizeof(ComponentListHeaderHot));
                    std::memcpy(newPageListsHot, m_pageListsHot, m_sceneDenseCount * componentSetup.pageListCount * sizeof(PageListHeaderHot));
                    std::memcpy(newComponentListsCold, m_componentListsCold, m_sceneDenseCount * componentSetup.slotListCount * sizeof(ComponentListHeaderCold));
                    std::memcpy(newPageListsCold, m_pageListsCold, m_sceneDenseCount * componentSetup.pageListCount * sizeof(ComponentListHeaderCold));
                    std::memcpy(newSceneData, m_sceneData, m_sceneDenseCount * sizeof(SceneData));
                }
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

        m_sceneSlots = reinterpret_cast<SceneSlot*>(m_scenes + sceneSlotOffset);
        m_componentListsHot = reinterpret_cast<ComponentListHeaderHot*>(m_scenes + componentListHotOffset);
        m_pageListsHot = reinterpret_cast<PageListHeaderHot*>(m_scenes + pageListHotOffset);
        m_componentListsCold = reinterpret_cast<ComponentListHeaderCold*>(m_scenes + componentListColdOffset);
        m_pageListsCold = reinterpret_cast<PageListHeaderCold*>(m_scenes + pageListColdOffset);
        m_sceneData = reinterpret_cast<SceneData*>(m_scenes + sceneDataOffset);
    }
}