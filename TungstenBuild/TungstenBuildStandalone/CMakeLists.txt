cmake_minimum_required(VERSION 3.14)

project(TungstenBuildStandalone VERSION ${TUNGSTEN_VERSION} LANGUAGES CXX)

message(STATUS "TungstenBuildStandalone version: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/../../cmake/Version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/TungstenBuildStandalone/Version.hpp
    @ONLY
)

# Add CLI11
set(CLI11_INSTALL OFF CACHE BOOL "Disable CLI11 install target")
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/CLI11)

add_executable(TungstenBuildStandalone
    src/main.cpp
)

target_compile_features(TungstenBuildStandalone PUBLIC cxx_std_20)

target_link_libraries(TungstenBuildStandalone PRIVATE
    TungstenBuild
    CLI11::CLI11
)

set_target_properties(TungstenBuildStandalone PROPERTIES
    OUTPUT_NAME "tungstenbuild"
    VERSION ${PROJECT_VERSION}
)

add_custom_command(TARGET TungstenBuildStandalone POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf
            "$<TARGET_FILE_DIR:TungstenBuildStandalone>/tungstenbuild_data"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_LIST_DIR}/../TungstenBuild/res/tungstenbuild_data"
            "$<TARGET_FILE_DIR:TungstenBuildStandalone>/tungstenbuild_data"
)