cmake_minimum_required(VERSION 3.14)

include("${CMAKE_CURRENT_LIST_DIR}/../cmake/TungstenVersion.cmake")
message(STATUS "Tungsten version: ${TUNGSTEN_VERSION}")

project(TungstenRuntime VERSION ${TUNGSTEN_VERSION} LANGUAGES CXX)

message(STATUS "TungstenRuntime version: ${PROJECT_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

add_executable(TungstenRuntime
    src/main.cpp
)

target_include_directories(TungstenRuntime PUBLIC
    ${INT_DIR}/include
)

target_compile_features(TungstenRuntime PUBLIC cxx_std_20)

target_link_libraries(TungstenRuntime PRIVATE Tungsten::ActiveProject)

set_target_properties(TungstenRuntime PROPERTIES OUTPUT_NAME "${TUNGSTEN_RUNTIME_EXECUTABLE_NAME}")