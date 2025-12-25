cmake_minimum_required(VERSION 3.14)

project(TungstenBuild VERSION ${TUNGSTEN_VERSION} LANGUAGES CXX)

message(STATUS "TungstenBuild version: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/../../cmake/Version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/TungstenBuild/Version.hpp
    @ONLY
)

# Add Rapid YAML
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/rapidlyaml)

# Add CSV Parser
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/csv-parser)

# Add TungstenUtils
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../external/TungstenUtils ${CMAKE_BINARY_DIR}/external/TungstenUtils)

add_library(TungstenBuild STATIC
    include/TungstenBuild/TungstenBuild.hpp
    src/TungstenBuild.cpp
)

target_include_directories(TungstenBuild PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)

target_compile_features(TungstenBuild PUBLIC cxx_std_20)

target_link_libraries(TungstenBuild
    PUBLIC TungstenUtils
    PRIVATE ryml
    PRIVATE csv
)

set_target_properties(TungstenBuild PROPERTIES
    VERSION ${PROJECT_VERSION}
)