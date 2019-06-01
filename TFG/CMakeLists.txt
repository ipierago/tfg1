# Subdirectories
add_subdirectory(Foo)
add_subdirectory(Bar)
add_subdirectory(FooBar)

#add_subdirectory(Call)
add_subdirectory(NetNT)
add_subdirectory(MayaPlugin)

set(MY_NAME TFG)

set(SOURCE
  error.cpp
  TFG.cpp
)

set(HEADERS
  djb2.h
  error.h
  Tfg_fwd.h
  Sys.h
  TFG.h  
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

add_precompiled_header(${MY_NAME} TFG.h)

if ((IOS AND BUILD_SHARED_LIBS))
  mylibrary_setupframework(${MY_NAME})
endif()

if (WIN32)
	target_link_libraries(${MY_NAME} PUBLIC wsock32 ws2_32)

	if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
		# Win64 stuff
	else()
		find_library(FIND_LIBRARY_LIBEGL libEGL)
		find_library(FIND_LIBRARY_LIBGLESV2 libGLESv2)
		target_link_libraries(${MY_NAME} PUBLIC ${FIND_LIBRARY_LIBEGL} ${FIND_LIBRARY_LIBGLESV2})
	endif()
endif()

if (ANDROID)
	target_link_libraries(${MY_NAME} PUBLIC android log EGL GLESv2)
endif()

if (IOS)
#	if (BUILD_SHARED_LIBS)
		find_library(FIND_LIBRARY_OPENGLES OpenGLES)
		target_link_libraries(${MY_NAME} PUBLIC ${FIND_LIBRARY_OPENGLES})
#	endif()
endif()
