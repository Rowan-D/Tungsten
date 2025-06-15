cmake_minimum_required(VERSION 3.14)
project(TungstenUtils LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/vendor/fmt)

add_library(TungstenUtils
    include/TungstenUtils.hpp
    include/TungstenLogger.hpp
    src/TungstenLogger.cpp
)

target_include_directories(TungstenUtils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(TungstenUtils PUBLIC fmt::fmt)