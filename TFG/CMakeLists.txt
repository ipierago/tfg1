# Subdirectories
add_subdirectory(Foo)
add_subdirectory(Bar)
add_subdirectory(FooBar)

#add_subdirectory(Call)
add_subdirectory(NetNT)

set(MY_NAME TFG)

set(SOURCE
  error.cpp
  TFG.cpp
)

set(HEADERS
  djb2.h
  error.h
  Tfg_fwd.h
  Sys.h
  TFG.h  
  types.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

add_precompiled_header(${MY_NAME} TFG.h)
