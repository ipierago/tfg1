cmake_minimum_required(VERSION 3.4.1)

set (MY_NAME tfg_netnt_tests)

set(SOURCE
  src/tests.cpp
)

set(HEADERS
  
)

add_executable(${MY_NAME} ${SOURCE} ${HEADERS})

#include_directories ("${PROJECT_SOURCE_DIR}/googletest/googletest/include")

target_include_directories(${MY_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/googletest/googletest/include")

target_link_libraries(${MY_NAME} tfg_netnt gtest_main)
