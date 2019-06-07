#pragma once
#ifdef WIN32

#include <Call/call_fwd.h>
#include <Sys/sys.h>

CALL_API BOOL call_RIOSend(PRIO_EXTENSION_FUNCTION_TABLE vtbl, RIO_RQ SocketQueue, PRIO_BUF pData, ULONG DataBufferCount, DWORD Flags, PVOID RequestContext);
CALL_API BOOL call_RIOSendEx(PRIO_EXTENSION_FUNCTION_TABLE vtbl, RIO_RQ SocketQueue, PRIO_BUF pData, ULONG DataBufferCount, PRIO_BUF pLocalAddress, PRIO_BUF pRemoteAddress, PRIO_BUF pControlContext, PRIO_BUF pFlags, DWORD Flags, PVOID RequestContext);
CALL_API BOOL call_RIOReceive(PRIO_EXTENSION_FUNCTION_TABLE vtbl, RIO_RQ SocketQueue, PRIO_BUF pData, ULONG DataBufferCount, DWORD Flags, PVOID RequestContext);
CALL_API RIO_RQ call_RIOCreateRequestQueue(PRIO_EXTENSION_FUNCTION_TABLE vtbl, SOCKET Socket, ULONG MaxOutstandingReceive, ULONG MaxReceiveDataBuffers, ULONG MaxOutstandingSend, ULONG MaxSendDataBuffers, RIO_CQ ReceiveCQ, RIO_CQ SendCQ, PVOID SocketContext);
CALL_API RIO_BUFFERID call_RIORegisterBuffer(PRIO_EXTENSION_FUNCTION_TABLE vtbl, PCHAR DataBuffer, DWORD DataLength);
CALL_API INT call_RIONotify(PRIO_EXTENSION_FUNCTION_TABLE vtbl, RIO_CQ CQ);
#endif
