set(MY_NAME TFG_NetNT)

set(SOURCE
  NetNT.cpp
  ObjectTracker.cpp
  Packet.cpp
  Singleton.cpp
  StreamToDGram.cpp
  #TCPAcceptor.cpp
  TCPConnection.cpp
  TCPConnector.cpp  
  ThreadSingleton.cpp
  UDPSocket.cpp
)

set(HEADERS
  NetNT.h
  ObjectTracker.h
  OverlappedEx.h
  Packet.h
  Singleton.h
  StreamToDGram.h
  #TCPAcceptor.h
  TCPConnection.h
  TCPConnector.h
  ThreadSingleton.h
  UDPSocket.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

target_link_libraries(${MY_NAME} TFG)

add_precompiled_header(${MY_NAME} NetNT.h)

add_subdirectory(tests)
