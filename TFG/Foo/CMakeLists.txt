set(MY_NAME TFG_Foo)

set(SOURCE
  Foo.cpp
)

set(HEADERS
  Foo.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

add_precompiled_header(${MY_NAME} Foo.h)
