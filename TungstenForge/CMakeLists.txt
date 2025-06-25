cmake_minimum_required(VERSION 3.14)
project(TungstenForge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(TungstenForge
    include/TungstenForge.hpp
    src/TungstenForge.cpp
)

target_include_directories(TungstenForge PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(TungstenForge PUBLIC TungstenUtils)