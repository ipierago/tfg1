cmake_minimum_required(VERSION 3.4.1)

set (MY_NAME TFG_NetNT_Test)

set(SOURCE
  Test.cpp
)

if (WIN32)
  set(SOURCE ${SOURCE}
    main.cpp
  )
endif()

set(HEADERS
  Test.h
)

add_executable(${MY_NAME} ${SOURCE} ${HEADERS})

target_link_libraries(${MY_NAME} TFG_NetNT)

add_precompiled_header(${MY_NAME} Test.h)
