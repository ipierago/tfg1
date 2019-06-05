set(MY_NAME tfg_bar)

set(SOURCE
  src/bar.cpp
  src/myobject.cpp
)

set(HEADERS
  include/tfg/bar/bar.h
  include/tfg/bar/myobject.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

target_include_directories(${MY_NAME}
        PRIVATE src include/tfg/bar
        PUBLIC include)

target_link_libraries(${MY_NAME} tfg)
