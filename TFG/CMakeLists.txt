# Subdirectories
add_subdirectory(Foo)
add_subdirectory(Bar)
add_subdirectory(FooBar)

set(MY_NAME TFG)

set(SOURCE
  TFG.cpp
)

set(HEADERS
  TFG.h  
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

add_precompiled_header(${MY_NAME} TFG.h)
