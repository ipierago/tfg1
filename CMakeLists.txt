cmake_minimum_required(VERSION 3.4.1)

# Extensions
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(PrecompiledHeader)

project (Tfg)

# Build shared libs by default
set(BUILD_SHARED_LIBS TRUE)

# On Windows builds use cmake magic to avoid __declpec
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

# Make sure shared libraries end up next to executables
# TODO: Needed?
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# Always include the root folder
# TODO: Should this be pushed into modules?
include_directories ("${PROJECT_SOURCE_DIR}")

# Subdirectories
add_subdirectory(Tfg)
