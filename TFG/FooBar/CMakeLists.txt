cmake_minimum_required(VERSION 3.4.1)

set (MY_NAME TFG_FooBar)

set(SOURCE
  FooBar.cpp
)

set(HEADERS
 
)

add_executable(${MY_NAME} ${SOURCE} ${HEADERS})

target_link_libraries(${MY_NAME} TFG_Foo)
target_link_libraries(${MY_NAME} TFG_Bar)
