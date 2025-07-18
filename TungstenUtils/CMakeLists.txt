cmake_minimum_required(VERSION 3.14)
project(TungstenUtils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add fmt
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/vendor/fmt)

add_library(TungstenUtils
    include/TungstenUtils/TungstenUtils.hpp
    include/TungstenUtils/TungstenLogger.hpp
    include/TungstenUtils/SlotList.hpp
    include/macros/noOp.hpp
    include/macros/debugBreak.hpp
    include/macros/assert.hpp
    src/wUtilsPCH.hpp
    src/wUtilsPCH.cpp
    src/TungstenLogger.cpp
)

target_include_directories(TungstenUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#target_precompile_headers(TungstenUtils PRIVATE TungstenUtils/wUtilsPCH.hpp)

target_link_libraries(TungstenUtils PUBLIC fmt::fmt)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# Enable tests only if requested
include(CTest)  # Sets BUILD_TESTING option
if (BUILD_TESTING)
    add_subdirectory(tests)
endif()