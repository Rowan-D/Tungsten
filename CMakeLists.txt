cmake_minimum_required(VERSION 3.14)

project(Tungsten LANGUAGES CXX)

enable_testing()

# Add projects
add_subdirectory(TungstenUtils)
add_subdirectory(TungstenCore)
add_subdirectory(TungstenForge)
add_subdirectory(TungstenForgeCLI)