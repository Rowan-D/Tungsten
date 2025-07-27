cmake_minimum_required(VERSION 3.14)
project(SandBox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SandBox
    include/SandBox/SandBox.hpp
    src/SandBox.cpp
)

target_include_directories(SandBox PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SandBox PUBLIC TungstenCore)