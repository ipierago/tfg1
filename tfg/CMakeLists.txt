set(MY_NAME tfg)

set(SOURCE
  src/error.cpp
  src/tfg.cpp
  stdafx.cpp
)

set(HEADERS
  include/tfg/djb2.h
  include/tfg/error.h
  include/tfg/tfg_fwd.h
  include/tfg/sys.h
  include/tfg/tfg.h  
  src/stdafx.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

target_include_directories(${MY_NAME}
        PRIVATE src include/tfg
        PUBLIC include)

add_precompiled_header(${MY_NAME} stdafx.h)

if ((IOS AND BUILD_SHARED_LIBS))
  mylibrary_setupframework(${MY_NAME})
endif()

#if (WIN32)
#	if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
#		# Win64 stuff
#	else()
#		find_library(FIND_LIBRARY_LIBEGL libEGL)
#		find_library(FIND_LIBRARY_LIBGLESV2 libGLESv2)
#		target_link_libraries(${MY_NAME} PUBLIC ${FIND_LIBRARY_LIBEGL} ${FIND_LIBRARY_LIBGLESV2})
#	endif()
#endif()

#if (ANDROID)
#	target_link_libraries(${MY_NAME} PUBLIC android log EGL GLESv2)
#endif()

#if (IOS)
##	if (BUILD_SHARED_LIBS)
#		find_library(FIND_LIBRARY_OPENGLES OpenGLES)
#		target_link_libraries(${MY_NAME} PUBLIC ${FIND_LIBRARY_OPENGLES})
##	endif()
#endif()
