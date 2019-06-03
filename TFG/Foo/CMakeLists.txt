set(MY_NAME TFG_Foo)

set(SOURCE
  MyObject.cpp
  Foo.cpp
  MyOtherObject.cpp
  IMyOtherObject.cpp
  stdafx.cpp
)

set(HEADERS
  Foo.h
  MyObject.h
  MyOtherObject.h
  IMyOtherObject.h
  stdafx.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

add_precompiled_header(${MY_NAME} stdafx.h)
