#include <catch2/catch_all.hpp>
#include "TungstenUtils/SlotList.hpp"

TEST_CASE("SlotList Add and Retrieve", "[SlotList]") {
    wUtils::SlotList<int> map;

    SECTION("Add single item and retrieve by At") {
        uint32_t id = map.Add(42);
        REQUIRE(map.At(id) == 42);
    }

    SECTION("Add multiple items and access via operator[]") {
        uint32_t a = map.Add(100);
        uint32_t b = map.Add(200);
        uint32_t c = map.Add(300);

        REQUIRE(map[a] == 100);
        REQUIRE(map[b] == 200);
        REQUIRE(map[c] == 300);
    }
}
