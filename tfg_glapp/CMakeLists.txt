cmake_minimum_required(VERSION 3.4.1)

set (MY_NAME tfg_glapp)

set(SOURCE
  src/client.c
  src/esshader.c
  src/esshapes.c
  src/estransform.c
  src/esutil.c
)

set(HEADERS
  src/client.h
  src/esutil.h
)

if (WIN32)
  set(SOURCE ${SOURCE}
    src/main.c
	src/esutil_win32.c
  )
  set(HEADERS ${HEADERS}
	src/esutil_win.h
  )
endif()

if (ANDROID)
  set(SOURCE ${SOURCE}
    src/esutil_android.c
	src/android_main.c
  )
endif()

add_executable(${MY_NAME} ${SOURCE} ${HEADERS})

target_include_directories(${MY_NAME} PRIVATE src)

if (WIN32)
	target_link_libraries(${MY_NAME} PUBLIC ${FIND_LIBRARY_LIBEGL} ${FIND_LIBRARY_LIBGLESV2})
	target_compile_definitions(${MY_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
	add_custom_command(TARGET ${MY_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FIND_DLL_LIBEGL} $<TARGET_FILE_DIR:${MY_NAME}>)
	add_custom_command(TARGET ${MY_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${FIND_DLL_LIBGLESV2} $<TARGET_FILE_DIR:${MY_NAME}>)
endif()

if (ANDROID)
	target_include_directories(${MY_NAME} PRIVATE ${ANDROID_NDK}/sources/android/native_app_glue)
	find_library(FIND_LIBRARY_ANDROID android)
	find_library(FIND_LIBRARY_LOG log)
	find_library(FIND_LIBRARY_EGL EGL)
	find_library(FIND_LIBRARY_GLESV2 GLESv2)
	target_link_libraries(${MY_NAME} ${FIND_LIBRARY_ANDROID} ${FIND_LIBRARY_LOG} ${FIND_LIBRARY_EGL} ${FIND_LIBRARY_GLESV2})
endif()

if (IOS)
	target_compile_definitions(${MY_NAME} PRIVATE GLES_SILENCE_DEPRECATION)
	if (BUILD_SHARED_LIBS)
		find_library(FIND_LIBRARY_OPENGLES OpenGLES)
		if (NOT FIND_LIBRARY_OPENGLES)
			message(FATAL_ERROR, "OpenGLES library not found")
		endif()
		target_link_libraries(${MY_NAME} ${FIND_LIBRARY_OPENGLES})
	endif()
endif()
