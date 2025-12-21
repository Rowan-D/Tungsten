cmake_minimum_required(VERSION 3.14)

include("${CMAKE_CURRENT_LIST_DIR}/../cmake/TungstenVersion.cmake")
message(STATUS "Tungsten version: ${TUNGSTEN_VERSION}")

project(TungstenCore VERSION ${TUNGSTEN_VERSION} LANGUAGES CXX)

message(STATUS "TungstenCore version: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/TungstenCore/Version.hpp
    @ONLY
)

# Add TungstenUtils
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../external/TungstenUtils ${CMAKE_BINARY_DIR}/external/TungstenUtils)

add_library(TungstenCore
    include/TungstenCore/TungstenCore.hpp
    include/TungstenCore/Application.hpp
    include/TungstenCore/ComponentSystem.hpp
    include/TungstenCore/ComponentSetup.hpp
    src/wCorePCH.cpp
    src/Application.cpp
    src/ComponentSystem.cpp
    src/ComponentSetup.cpp
)

target_include_directories(TungstenCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_compile_features(TungstenCore PUBLIC cxx_std_20)
target_precompile_headers(TungstenCore PRIVATE src/wCorePCH.hpp)

target_link_libraries(TungstenCore PUBLIC TungstenUtils)

set_target_properties(TungstenCore PROPERTIES
    VERSION ${PROJECT_VERSION}
)