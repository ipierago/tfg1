cmake_minimum_required(VERSION 3.4.1)

# Extensions
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(add_precompiled_header)

project (tfg)

# Build shared libs by default
set(BUILD_SHARED_LIBS TRUE)

# On Windows builds use cmake magic to avoid __declpec
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

# OSX - supress warning about rpath
cmake_policy(SET CMP0042 NEW)

## Make sure shared libraries end up next to executables
## TODO: Is this needed with static link?
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Always include the root folder
include_directories ("${PROJECT_SOURCE_DIR}")

# OpenGL
if (WIN32)
    # PowerVR
    include_directories("C:\\Imagination\\PowerVR_Graphics\\PowerVR_SDK\\SDK_2018_R2\\include")
	if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
		set(FIND_DLL_LIBEGL "C:\\Imagination\\PowerVR_Graphics\\PowerVR_Tools\\PVRVFrame\\Library\\Windows_x86_64\\libEGL.dll")
		set(FIND_DLL_LIBGLESV2 "C:\\Imagination\\PowerVR_Graphics\\PowerVR_Tools\\PVRVFrame\\Library\\Windows_x86_64\\libGLESv2.dll")	
		find_library(FIND_LIBRARY_LIBEGL NAMES libEGL HINTS "C:\\Imagination\\PowerVR_Graphics\\PowerVR_Tools\\PVRVFrame\\Library\\Windows_x86_64")
		find_library(FIND_LIBRARY_LIBGLESV2 NAMES libGLESv2 HINTS "C:\\Imagination\\PowerVR_Graphics\\PowerVR_Tools\\PVRVFrame\\Library\\Windows_x86_64")
	else()
		find_library(FIND_LIBRARY_LIBEGL NAMES libEGL HINTS "C:\\Imagination\\PowerVR_Graphics\\PowerVR_Tools\\PVRVFrame\\Library\\Windows_x86_32")
		find_library(FIND_LIBRARY_LIBGLESV2 NAMES libGLESv2 HINTS "C:\\Imagination\\PowerVR_Graphics\\PowerVR_Tools\\PVRVFrame\\Library\\Windows_x86_32")
		set(FIND_DLL_LIBEGL "C:\\Imagination\\PowerVR_Graphics\\PowerVR_Tools\\PVRVFrame\\Library\\Windows_x86_32\\libEGL.dll")
		set(FIND_DLL_LIBGLESV2 "C:\\Imagination\\PowerVR_Graphics\\PowerVR_Tools\\PVRVFrame\\Library\\Windows_x86_32\\libGLESv2.dll")	
	endif()
#   # AdrenoSDK
#   include_directories("C:\\AdrenoSDK\\Development\\Inc")
#	if(CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
#		find_library(FIND_LIBRARY_LIBEGL NAMES libEGL HINTS "C:\\AdrenoSDK\\Development\\Lib\\x64")
#		find_library(FIND_LIBRARY_LIBGLESV2 NAMES libGLESv2 HINTS "C:\\AdrenoSDK\\Development\\Lib\\x64")
#	else()
#		find_library(FIND_LIBRARY_LIBEGL NAMES libEGL HINTS "C:\\AdrenoSDK\\Development\\Lib\\Win32")
#		find_library(FIND_LIBRARY_LIBGLESV2 NAMES libGLESv2 HINTS "C:\\AdrenoSDK\\Development\\Lib\\Win32")
#	endif()
endif()  # WIN32

# dependencies subdirectories
add_subdirectory(thirdparty/googletest)

# subdirectories
add_subdirectory(tfg)
add_subdirectory(tfg_foo)
add_subdirectory(tfg_bar)
add_subdirectory(tfg_foobar)
add_subdirectory(tfg_mayaplugin)
add_subdirectory(tfg_netnt)
add_subdirectory(tfg_glapp)