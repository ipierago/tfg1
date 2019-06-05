set(MY_NAME tfg_netnt)

set(SOURCE
  src/netnt.cpp
  src/objecttracker.cpp
  src/packet.cpp
  src/singleton.cpp
  src/streamtodgram.cpp
  src/tcpacceptor.cpp
  src/tcpconnection.cpp
  src/tcpconnector.cpp  
  src/threadsingleton.cpp
  src/udpsocket.cpp
  stdafx.cpp
)

set(HEADERS
  include/tfg/netnt/netnt.h
  include/tfg/netnt/objecttracker.h
  include/tfg/netnt/overlappedex.h  
  include/tfg/netnt/packet.h
  include/tfg/netnt/singleton.h
  include/tfg/netnt/streamtodgram.h
  include/tfg/netnt/tcpacceptor.h
  include/tfg/netnt/tcpconnection.h
  include/tfg/netnt/tcpconnector.h
  include/tfg/netnt/threadsingleton.h
  include/tfg/netnt/udpsocket.h
  src/stdafx.h
)

add_library (${MY_NAME} ${SOURCE} ${HEADERS})

target_include_directories(${MY_NAME}
        PRIVATE src include/tfg/netnt
        PUBLIC include)

target_link_libraries(${MY_NAME} PUBLIC tfg)
target_link_libraries(${MY_NAME} PUBLIC wsock32 ws2_32)

#add_precompiled_header(${MY_NAME} stdafx.h)

add_subdirectory(tests)
