set(MY_NAME tfg_log)

set(SOURCE
  src/log.cpp
  stdafx.cpp
)

set(HEADERS
  include/tfg/log/log.h
  src/stdafx.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

target_link_libraries(${MY_NAME} PUBLIC tfg)

target_include_directories(${MY_NAME}
        PRIVATE src 
        PUBLIC include)

add_precompiled_header(${MY_NAME} stdafx.h)

if ((IOS AND BUILD_SHARED_LIBS))
  mylibrary_setupframework(${MY_NAME})
endif()
