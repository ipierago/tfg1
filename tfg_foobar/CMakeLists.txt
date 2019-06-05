cmake_minimum_required(VERSION 3.4.1)

set (MY_NAME tfg_foobar)

set(SOURCE
  src/foobar.cpp
)

if (WIN32)
  set(SOURCE ${SOURCE}
    src/main.cpp
  )
endif()

set(HEADERS
  src/foobar.h
)

add_executable(${MY_NAME} ${SOURCE} ${HEADERS})

target_include_directories(${MY_NAME}
        PRIVATE src
        PUBLIC include)

target_link_libraries(${MY_NAME} tfg tfg_foo tfg_bar)

