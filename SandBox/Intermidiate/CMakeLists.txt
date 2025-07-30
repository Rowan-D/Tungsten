cmake_minimum_required(VERSION 3.14)

project(TungstenProject LANGUAGES CXX)

# Add projects
add_subdirectory(/home/rowan/dev/Tungsten/SandBox/Engine/TungstenCore ${CMAKE_BINARY_DIR}/TungstenCore)
add_subdirectory(/home/rowan/dev/Tungsten/SandBox/SandBox ${CMAKE_BINARY_DIR}/SandBox)
add_subdirectory(TungstenReflect)
add_subdirectory(TungstenRuntime)