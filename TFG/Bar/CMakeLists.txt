set(MY_NAME TFG_Bar)

set(SOURCE
  Bar.cpp
  MyObject.cpp
)

set(HEADERS
  Bar.h
  MyObject.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

target_link_libraries(${MY_NAME} TFG)

add_precompiled_header(${MY_NAME} Bar.h)
