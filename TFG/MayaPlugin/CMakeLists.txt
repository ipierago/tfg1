
set(MY_NAME TFG_MayaPlugin)

set(SOURCE
  mllmain.cpp
  stdafx.cpp
)

set(HEADERS
  stdafx.h
)

include_directories ("$ENV{DEVKIT_LOCATION}/include")

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

target_compile_definitions(${MY_NAME} PRIVATE NT_PLUGIN)

find_library(FIND_LIBRARY_OPENMAYA NAMES OpenMaya HINTS "$ENV{DEVKIT_LOCATION}/lib" )
target_link_libraries(${MY_NAME} PUBLIC ${FIND_LIBRARY_OPENMAYA})
find_library(FIND_LIBRARY_OPENMAYAUI NAMES OpenMayaUI HINTS "$ENV{DEVKIT_LOCATION}/lib" )
target_link_libraries(${MY_NAME} PUBLIC ${FIND_LIBRARY_OPENMAYAUI})
find_library(FIND_LIBRARY_OPENMAYARENDER NAMES OpenMayaRender HINTS "$ENV{DEVKIT_LOCATION}/lib" )
target_link_libraries(${MY_NAME} PUBLIC ${FIND_LIBRARY_OPENMAYARENDER})
find_library(FIND_LIBRARY_FOUNDATION NAMES Foundation HINTS "$ENV{DEVKIT_LOCATION}/lib" )
target_link_libraries(${MY_NAME} PUBLIC ${FIND_LIBRARY_FOUNDATION})

#target_link_libraries(${MY_NAME} PUBLIC TFG)

set_target_properties(${MY_NAME} PROPERTIES SUFFIX ".mll")

add_precompiled_header(${MY_NAME} stdafx.h)
