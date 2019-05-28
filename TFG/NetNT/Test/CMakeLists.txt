cmake_minimum_required(VERSION 3.4.1)

set (MY_NAME TFG_NetNT_Test)

set(SOURCE
  main.cpp
)

set(HEADERS
  
)

add_executable(${MY_NAME} ${SOURCE} ${HEADERS})

target_link_libraries(${MY_NAME} TFG_NetNT)
