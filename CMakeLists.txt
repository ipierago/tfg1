cmake_minimum_required(VERSION 3.4.1)

# Extensions
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(add_precompiled_header)

project (tfg)

# Build shared libs by default
set(BUILD_SHARED_LIBS TRUE)

# On Windows builds use cmake magic to avoid __declpec
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

## Make sure shared libraries end up next to executables
## TODO: Is this needed with static link?
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Always include the root folder
include_directories ("${PROJECT_SOURCE_DIR}")

# dependencies subdirectories
add_subdirectory(googletest)

# subdirectories
add_subdirectory(tfg)
add_subdirectory(tfg_foo)
add_subdirectory(tfg_bar)
add_subdirectory(tfg_foobar)
add_subdirectory(tfg_mayaplugin)
add_subdirectory(tfg_netnt)
