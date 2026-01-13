#include "wCorePCH.hpp"
#include "TungstenCore/ComponentSystem.hpp"

namespace wCore
{
    ComponentSystem::ComponentSystem(Application& app) noexcept
        : m_app(app),
        m_sceneMemory{}, m_sceneList{}
    {
    }

    ComponentSystem::~ComponentSystem() noexcept
    {
    }

    /*SceneHandle ComponentSystem::CreateScene()
    {
        if (m_sceneFreeList.Empty())
        {
            if (m_sceneSlotCount == m_sceneCapacity)
            {
                ReallocateScenes(CalculateNextCapacity(m_sceneCapacity));
            }

            const std::size_t sceneStartComponentIndex = m_sceneDenseCount * componentSetup.slotListCount;
            const std::size_t sceneStartPageIndex = m_sceneDenseCount * componentSetup.pageListCount;

            std::memset(m_componentListsHot + sceneStartComponentIndex, 0, sizeof(void*) * componentSetup.slotListCount);
            std::memset(m_pageListsHot + sceneStartPageIndex, 0, sizeof(void*) * componentSetup.pageListCount);
            std::memset(m_componentListsCold + sceneStartComponentIndex, 0, sizeof(ComponentListHeaderCold) * componentSetup.slotListCount);
            std::memset(m_pageListsCold + sceneStartPageIndex, 0, sizeof(PageListHeaderCold) * componentSetup.pageListCount);
            std::construct_at(m_sceneData + m_sceneDenseCount);

            m_sceneSlotToDense[m_sceneSlotCount] = m_sceneDenseCount++;

            ++m_sceneSlotCount;
            if (m_sceneSlotCount < m_sceneSlotGenerationCount)
            {
                return SceneHandle(m_sceneSlotCount, m_sceneGenerations[m_sceneSlotCount - sceneIndexStart]);
            }
            else
            {
                return SceneHandle(m_sceneSlotCount, *std::construct_at(m_sceneGenerations + m_sceneSlotCount - sceneIndexStart));
            }
        }
        const SceneIndex sceneIndex = m_sceneFreeList.Remove();

        const std::size_t sceneStartComponentIndex = m_sceneDenseCount * componentSetup.slotListCount;
        const std::size_t sceneStartPageIndex = m_sceneDenseCount * componentSetup.pageListCount;

        std::memset(m_componentListsHot + sceneStartComponentIndex, 0, sizeof(void*) * componentSetup.slotListCount);
        std::memset(m_pageListsHot + sceneStartPageIndex, 0, sizeof(void*) * componentSetup.pageListCount);
        std::memset(m_componentListsCold + sceneStartComponentIndex, 0, sizeof(ComponentListHeaderCold) * componentSetup.slotListCount);
        std::memset(m_pageListsCold + sceneStartPageIndex, 0, sizeof(PageListHeaderCold) * componentSetup.pageListCount);
        std::construct_at(m_sceneData + m_sceneDenseCount);

        m_sceneSlotToDense[sceneIndex - sceneIndexStart] = m_sceneDenseCount++;

        return SceneHandle(sceneIndex, m_sceneGenerations[sceneIndex - sceneIndexStart]);
    }*/

    /*void DestroyScene(SceneIndex sceneIndex) m_sceneGenerationsnoexcept
    {
    }

    void DestroyScene(SceneHandle sceneHandle) noexcept;
    {
    }*/

    /*void ComponentSystem::ReallocateScenes(wIndex newCapacity)
    {
        m_sceneCapacity = newCapacity;

        std::size_t offset = 0;

        offset = wUtils::AlignUp(offset, alignof(void*));
        const std::size_t componentListHotOffset = offset;
        offset += componentSetup.slotListCount * sizeof(void*) * newCapacity;

        offset = wUtils::AlignUp(offset, alignof(void*));
        const std::size_t pageListHotOffset = offset;
        offset += componentSetup.pageListCount * sizeof(void*) * newCapacity;

        offset = wUtils::AlignUp(offset, alignof(SceneIndex));
        const std::size_t sceneSlotToDenseOffset = offset;
        offset += newCapacity * sizeof(SceneIndex);

        offset = wUtils::AlignUp(offset, alignof(SceneGeneration));
        const std::size_t sceneGenerationOffset = offset;
        offset += newCapacity * sizeof(SceneGeneration);

        offset = wUtils::AlignUp(offset, alignof(ComponentListHeaderCold));
        const std::size_t componentListColdOffset = offset;
        offset += componentSetup.slotListCount * sizeof(ComponentListHeaderCold) * newCapacity;

        offset = wUtils::AlignUp(offset, alignof(PageListHeaderCold));
        const std::size_t pageListColdOffset = offset;
        offset += componentSetup.pageListCount * sizeof(PageListHeaderCold) * newCapacity;

        offset = wUtils::AlignUp(offset, alignof(SceneData));
        const std::size_t sceneDataOffset = offset;
        offset += newCapacity * sizeof(SceneData);

        constexpr std::size_t alignment = wUtils::maxAlignOf_v<void*, SceneIndex, SceneGeneration, ComponentListHeaderCold, PageListHeaderCold, SceneData>;
        if (m_scenes)
        {
            std::byte* newScenes = static_cast<std::byte*>(
                ::operator new(offset, std::align_val_t(alignment))
            );

            void** newComponentListsHot = reinterpret_cast<void**>(newScenes + componentListHotOffset);
            void** newPageListsHot = reinterpret_cast<void**>(newScenes + pageListHotOffset);
            SceneIndex* newSceneSlotToDense = reinterpret_cast<SceneIndex*>(newScenes + sceneSlotToDenseOffset);
            SceneGeneration* newSceneGenerations = reinterpret_cast<SceneGeneration*>(newScenes + sceneGenerationOffset);
            ComponentListHeaderCold* newComponentListsCold = reinterpret_cast<ComponentListHeaderCold*>(newScenes + componentListColdOffset);
            PageListHeaderCold* newPageListsCold = reinterpret_cast<PageListHeaderCold*>(newScenes + pageListColdOffset);
            SceneData* newSceneData = reinterpret_cast<SceneData*>(newScenes + sceneDataOffset);

            if (m_sceneSlotGenerationCount)
            {
                std::memcpy(newSceneGenerations, m_sceneGenerations, m_sceneSlotGenerationCount * sizeof(SceneGeneration));
                if (m_sceneSlotCount)
                {
                    std::memcpy(newSceneSlotToDense, m_sceneSlotToDense, m_sceneSlotCount * sizeof(SceneIndex));
                    if (m_sceneDenseCount)
                    {
                        std::memcpy(newComponentListsHot, m_componentListsHot, m_sceneDenseCount * componentSetup.slotListCount * sizeof(void*));
                        std::memcpy(newPageListsHot, m_pageListsHot, m_sceneDenseCount * componentSetup.pageListCount * sizeof(void*));
                        std::memcpy(newComponentListsCold, m_componentListsCold, m_sceneDenseCount * componentSetup.slotListCount * sizeof(ComponentListHeaderCold));
                        std::memcpy(newPageListsCold, m_pageListsCold, m_sceneDenseCount * componentSetup.pageListCount * sizeof(ComponentListHeaderCold));
                        std::memcpy(newSceneData, m_sceneData, m_sceneDenseCount * sizeof(SceneData));
                    }
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

        m_componentListsHot = reinterpret_cast<void**>(m_scenes + componentListHotOffset);
        m_pageListsHot = reinterpret_cast<void**>(m_scenes + pageListHotOffset);
        m_sceneSlotToDense = reinterpret_cast<SceneIndex*>(m_scenes + sceneSlotToDenseOffset);
        m_sceneGenerations = reinterpret_cast<SceneGeneration*>(m_scenes + sceneGenerationOffset);
        m_componentListsCold = reinterpret_cast<ComponentListHeaderCold*>(m_scenes + componentListColdOffset);
        m_pageListsCold = reinterpret_cast<PageListHeaderCold*>(m_scenes + pageListColdOffset);
        m_sceneData = reinterpret_cast<SceneData*>(m_scenes + sceneDataOffset);
    }*/
}