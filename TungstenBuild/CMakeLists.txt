cmake_minimum_required(VERSION 3.14)

include("${CMAKE_CURRENT_LIST_DIR}/../cmake/TungstenVersion.cmake")
message(STATUS "Tungsten version: ${TUNGSTEN_VERSION}")

project(TungstenBuildWorkspace VERSION ${TUNGSTEN_VERSION} LANGUAGES CXX)

if(PROJECT_IS_TOP_LEVEL)
    # Multi-config generators
    if(CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_CONFIGURATION_TYPES
            Debug;Release;RelWithDebInfo;MinSizeRel;Dist
            CACHE STRING "Available configurations" FORCE)
    else()
        # Single-config generators
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
                     STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel;Dist")
        if(NOT CMAKE_BUILD_TYPE)
            set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
        endif()
    endif()

    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_Dist ON)
endif()

add_subdirectory(TungstenBuild)
add_subdirectory(TungstenBuildStandalone)
