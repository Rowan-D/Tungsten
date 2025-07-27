cmake_minimum_required(VERSION 3.14)
project(TungstenRuntime LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SandBoxRuntime
    src/generated/projectDefines.hpp
    src/main.cpp
)

target_link_libraries(SandBoxRuntime PRIVATE SandBox)

set_target_properties(SandBoxRuntime PROPERTIES OUTPUT_NAME "SandBox")