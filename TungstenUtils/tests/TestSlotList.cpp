#include <catch2/catch_all.hpp>
#include "TungstenUtils/SlotList.hpp"

#include <string>

TEST_CASE("Add and Remove Integers", "[SlotList]")
{
    wUtils::SlotList<int32_t> slotList;

    uint32_t index = wUtils::SlotList<int32_t>::Null;
    uint32_t indexA = wUtils::SlotList<int32_t>::Null;
    uint32_t indexB = wUtils::SlotList<int32_t>::Null;
    uint32_t indexC = wUtils::SlotList<int32_t>::Null;

    // Initial state is empty
    REQUIRE(slotList.Count() == 0);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    // Add Elements
    index = slotList.Add(42);
    REQUIRE(index != wUtils::SlotList<int32_t>::Null);
    REQUIRE(slotList.Count() == 1);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    REQUIRE(slotList[index] == 42);
    REQUIRE(slotList.At(index) == 42);
    REQUIRE(slotList.Exists(index));

    indexA = slotList.Add(100);
    REQUIRE(indexA != wUtils::SlotList<int32_t>::Null);
    REQUIRE(slotList.Count() == 2);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    indexB = slotList.Add(200);
    REQUIRE(indexB != wUtils::SlotList<int32_t>::Null);
    REQUIRE(slotList.Count() == 3);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    indexC = slotList.Add(300);
    REQUIRE(indexC != wUtils::SlotList<int32_t>::Null);
    REQUIRE(slotList.Count() == 4);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    REQUIRE(slotList[indexA] == 100);
    REQUIRE(slotList[indexB] == 200);
    REQUIRE(slotList[indexC] == 300);
    REQUIRE(slotList.At(indexA) == 100);
    REQUIRE(slotList.At(indexB) == 200);
    REQUIRE(slotList.At(indexC) == 300);
    REQUIRE(slotList.Exists(indexA));
    REQUIRE(slotList.Exists(indexB));
    REQUIRE(slotList.Exists(indexC));

    // Re Check initial value
    REQUIRE(slotList[index] == 42);
    REQUIRE(slotList.At(index) == 42);
    REQUIRE(slotList.Exists(index));

    // Remove Elements
    slotList.Remove(indexA);
    REQUIRE(slotList.Count() == 3);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(!slotList.Exists(indexA));

    slotList.Remove(indexB);
    REQUIRE(slotList.Count() == 2);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(!slotList.Exists(indexB));

    uint32_t indexD = wUtils::SlotList<int32_t>::Null;

    // Add Elements again
    indexD = slotList.Add(12345);
    REQUIRE(slotList.Exists(indexD));
    REQUIRE(slotList.Count() == 3);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexD] == 12345);
    REQUIRE(slotList.At(indexD) == 12345);

    indexA = slotList.Add(-12);
    REQUIRE(slotList.Exists(indexA));
    REQUIRE(slotList.Count() == 4);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexA] == -12);
    REQUIRE(slotList.At(indexA) == -12);

    indexB = slotList.Add(-12);
    REQUIRE(slotList.Exists(indexB));
    REQUIRE(slotList.Count() == 5);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexB] == -12);
    REQUIRE(slotList.At(indexB) == -12);

    // Clear
    slotList.Clear();
    REQUIRE(slotList.Count() == 0);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    // Add Elements again after Clear
    indexD = slotList.Add(1237864);
    REQUIRE(slotList.Exists(indexD));
    REQUIRE(slotList.Count() == 1);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexD] == 1237864);
    REQUIRE(slotList.At(indexD) == 1237864);

    indexA = slotList.Add(-2345);
    REQUIRE(slotList.Exists(indexD));
    REQUIRE(slotList.Count() == 2);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexA] == -2345);
    REQUIRE(slotList.At(indexA) == -2345);

    indexB = slotList.Add(-12);
    REQUIRE(slotList.Exists(indexB));
    REQUIRE(slotList.Count() == 3);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexB] == -12);
    REQUIRE(slotList.At(indexB) == -12);
}

TEST_CASE("Add and Remove Test Class With UseExistenceMethods", "[SlotList]")
{
    class ComponentWithExistenceMethods
    {
    public:
        ComponentWithExistenceMethods(const std::string& str, int32_t number)
            : m_str(str), m_number(number), m_exists(true)
        {
        }

        inline bool operator==(const ComponentWithExistenceMethods& other) const { return m_str == other.m_str && m_number == other.m_number; }

        inline bool Exists() const { return m_exists; }
        void SetExists(bool exists) { m_exists = exists; }

    private:
        std::string m_str;
        int32_t m_number;
        bool m_exists;
    };

    wUtils::SlotList<ComponentWithExistenceMethods> slotList;

    uint32_t index = wUtils::SlotList<ComponentWithExistenceMethods>::Null;
    uint32_t indexA = wUtils::SlotList<ComponentWithExistenceMethods>::Null;
    uint32_t indexB = wUtils::SlotList<ComponentWithExistenceMethods>::Null;
    uint32_t indexC = wUtils::SlotList<ComponentWithExistenceMethods>::Null;

    // Initial state is empty
    REQUIRE(slotList.Count() == 0);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    // Add Elements
    index = slotList.Add(ComponentWithExistenceMethods("Test str 42", 42));
    REQUIRE(slotList.Count() == 1);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    REQUIRE(slotList[index] == ComponentWithExistenceMethods("Test str 42", 42));
    REQUIRE(slotList.At(index) == ComponentWithExistenceMethods("Test str 42", 42));
    REQUIRE(slotList.Exists(index));

    indexA = slotList.Add(ComponentWithExistenceMethods("Test str 100", 100));
    REQUIRE(indexA != wUtils::SlotList<ComponentWithExistenceMethods>::Null);
    REQUIRE(slotList.Count() == 2);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    indexB = slotList.Add(ComponentWithExistenceMethods("Test str 200", 200));
    REQUIRE(indexB != wUtils::SlotList<ComponentWithExistenceMethods>::Null);
    REQUIRE(slotList.Count() == 3);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    indexC = slotList.Add(ComponentWithExistenceMethods("Test str 300", 300));
    REQUIRE(indexC != wUtils::SlotList<ComponentWithExistenceMethods>::Null);
    REQUIRE(slotList.Count() == 4);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    REQUIRE(slotList[indexA] == ComponentWithExistenceMethods("Test str 100", 100));
    REQUIRE(slotList[indexB] == ComponentWithExistenceMethods("Test str 200", 200));
    REQUIRE(slotList[indexC] == ComponentWithExistenceMethods("Test str 300", 300));
    REQUIRE(slotList.At(indexA) == ComponentWithExistenceMethods("Test str 100", 100));
    REQUIRE(slotList.At(indexB) == ComponentWithExistenceMethods("Test str 200", 200));
    REQUIRE(slotList.At(indexC) == ComponentWithExistenceMethods("Test str 300", 300));
    REQUIRE(slotList.Exists(indexA));
    REQUIRE(slotList.Exists(indexB));
    REQUIRE(slotList.Exists(indexC));

    // Re Check initial value
    REQUIRE(slotList[index] == ComponentWithExistenceMethods("Test str 42", 42));
    REQUIRE(slotList.At(index) == ComponentWithExistenceMethods("Test str 42", 42));
    REQUIRE(slotList.Exists(index));

    // Remove Elements
    slotList.Remove(indexA);
    REQUIRE(slotList.Count() == 3);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(!slotList.Exists(indexA));

    slotList.Remove(indexB);
    REQUIRE(slotList.Count() == 2);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(!slotList.Exists(indexB));

    uint32_t indexD = wUtils::SlotList<ComponentWithExistenceMethods>::Null;

    // Add Elements again
    indexD = slotList.Add(ComponentWithExistenceMethods("Test str 12345", 12345));
    REQUIRE(slotList.Exists(indexD));
    REQUIRE(slotList.Count() == 3);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexD] == ComponentWithExistenceMethods("Test str 12345", 12345));
    REQUIRE(slotList.At(indexD) == ComponentWithExistenceMethods("Test str 12345", 12345));

    indexA = slotList.Add(ComponentWithExistenceMethods("Test str -12", -12));
    REQUIRE(slotList.Exists(indexA));
    REQUIRE(slotList.Count() == 4);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexA] == ComponentWithExistenceMethods("Test str -12", -12));
    REQUIRE(slotList.At(indexA) == ComponentWithExistenceMethods("Test str -12", -12));

    indexB = slotList.Add(ComponentWithExistenceMethods("Test str -12", -12));
    REQUIRE(slotList.Exists(indexB));
    REQUIRE(slotList.Count() == 5);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexB] == ComponentWithExistenceMethods("Test str -12", -12));
    REQUIRE(slotList.At(indexB) == ComponentWithExistenceMethods("Test str -12", -12));

    // Clear
    slotList.Clear();
    REQUIRE(slotList.Count() == 0);
    REQUIRE(slotList.Count() <= slotList.SlotCount());

    // Add Elements again after Clear
    indexD = slotList.Add(ComponentWithExistenceMethods("Test str -1237864", -1237864));
    REQUIRE(slotList.Exists(indexD));
    REQUIRE(slotList.Count() == 1);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexD] == ComponentWithExistenceMethods("Test str -1237864", -1237864));
    REQUIRE(slotList.At(indexD) == ComponentWithExistenceMethods("Test str -1237864", -1237864));

    indexA = slotList.Add(ComponentWithExistenceMethods("Test str -1237864", -2345));
    REQUIRE(slotList.Exists(indexA));
    REQUIRE(slotList.Count() == 2);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexA] == ComponentWithExistenceMethods("Test str -1237864", -2345));
    REQUIRE(slotList.At(indexA) == ComponentWithExistenceMethods("Test str -1237864", -2345));

    indexB = slotList.Add(ComponentWithExistenceMethods("Test str -1237864", -12));
    REQUIRE(slotList.Exists(indexB));
    REQUIRE(slotList.Count() == 3);
    REQUIRE(slotList.Count() <= slotList.SlotCount());
    REQUIRE(slotList[indexB] == ComponentWithExistenceMethods("Test str -1237864", -12));
    REQUIRE(slotList.At(indexB) == ComponentWithExistenceMethods("Test str -1237864", -12));
}
