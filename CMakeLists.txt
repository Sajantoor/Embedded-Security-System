# CMake Build Configuration for root of project
cmake_minimum_required(VERSION 3.18)
project(security_project VERSION 1.0 DESCRIPTION "Project" LANGUAGES CXX)

# Compiler options (inherited by sub-folders)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
add_compile_options(-Wall -Werror -Wpedantic -Wextra -Wno-psabi)
add_compile_options(-fdiagnostics-color)

# Enable address sanitizer
# (Comment this out to make your code faster)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# What folders to build
add_subdirectory(common) 
add_subdirectory(hal)  
add_subdirectory(app)
