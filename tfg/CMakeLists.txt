set(MY_NAME tfg)

set(SOURCE
  src/error.cpp
  src/log.cpp
  src/tfg.cpp
  stdafx.cpp
)

set(HEADERS
  include/tfg/djb2.h
  include/tfg/error.h
  include/tfg/log.h
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

if (ANDROID)
	target_link_libraries(${MY_NAME} PUBLIC android log)
endif()
