cmake_minimum_required(VERSION 3.14)
project(SandBox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SandBox
    src/main.cpp
)

target_link_libraries(SandBox PUBLIC TungstenCore)