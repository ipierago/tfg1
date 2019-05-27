set (MY_NAME TFG_Call)

set(SOURCE
  call.cpp
  call_rio.cpp
  call_pch.cpp
)

set(HEADERS
  call.h
  call_clib.h
  call_rio.h
  call_winapi.h
  call_winsock.h
  call_pch.h 
  call_fwd.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

add_precompiled_header(${MY_NAME} call_pch.h)

target_link_libraries(${MY_NAME} TFG)
