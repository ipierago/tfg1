cmake_minimum_required(VERSION 3.4.1)

set (MY_NAME TFG_FooBar)

set(SOURCE
  FooBar.cpp
)

if (WIN32)
  set(SOURCE ${SOURCE}
    main.cpp
  )
endif()

set(HEADERS
  FooBar.h
)

add_executable(${MY_NAME} ${SOURCE} ${HEADERS})

target_link_libraries(${MY_NAME} TFG)
target_link_libraries(${MY_NAME} TFG_Foo)
target_link_libraries(${MY_NAME} TFG_Bar)

add_precompiled_header(${MY_NAME} FooBar.h)
