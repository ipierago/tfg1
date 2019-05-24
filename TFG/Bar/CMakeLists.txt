set(MY_NAME TFG_Bar)

set(SOURCE
  Bar.cpp
)

set(HEADERS
  Bar.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

add_precompiled_header(${MY_NAME} Bar.h)
