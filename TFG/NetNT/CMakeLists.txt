set(MY_NAME TFG_NetNT)

set(SOURCE
  NetNT.cpp
  Packet.cpp
)

set(HEADERS
  NetNT.h
  OverlappedEx.h
  Packet.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

add_precompiled_header(${MY_NAME} NetNT.h)
