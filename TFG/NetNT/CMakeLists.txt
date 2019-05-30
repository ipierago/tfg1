set(MY_NAME TFG_NetNT)

set(SOURCE
  NetNT.cpp
  Packet.cpp
  UDPSocket.cpp
  Singleton.cpp
  ObjectTracker.cpp
  ThreadSingleton.cpp
)

set(HEADERS
  NetNT.h
  OverlappedEx.h
  Packet.h
  UDPSocket.h
  Singleton.h
  ObjectTracker.h
  ThreadSingleton.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

target_link_libraries(${MY_NAME} TFG)

add_precompiled_header(${MY_NAME} NetNT.h)

add_subdirectory(tests)