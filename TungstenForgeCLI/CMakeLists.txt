cmake_minimum_required(VERSION 3.14)
project(TungstenForgeCLI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TungstenForgeCLI
    src/main.cpp
)

target_link_libraries(TungstenForgeCLI PUBLIC TungstenForge)