# Add test executable
add_executable(TungstenUtilsTests
    TestSlotList.cpp
)

target_link_libraries(TungstenUtilsTests PRIVATE
    TungstenUtils
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(TungstenUtilsTests)