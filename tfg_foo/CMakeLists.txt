set(MY_NAME tfg_foo)

set(SOURCE
  src/myobject.cpp
  src/foo.cpp
  src/myotherobject.cpp
  src/imyotherobject.cpp
  stdafx.cpp
)

set(HEADERS
  include/tfg/foo/foo.h
  include/tfg/foo/myobject.h
  include/tfg/foo/myotherobject.h
  include/tfg/foo/imyotherobject.h
  src/stdafx.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

target_include_directories(${MY_NAME}
        PRIVATE src include/tfg/foo
        PUBLIC include)

target_link_libraries(${MY_NAME} tfg tfg_log)

add_precompiled_header(${MY_NAME} stdafx.h)
