#include "call_pch.h"
#ifdef WIN32

#include <Call/call_rio.h>
#include <Std/Std.h>

STD_FILE_SETUP()


BOOL call_RIOSend(PRIO_EXTENSION_FUNCTION_TABLE vtbl, RIO_RQ SocketQueue, PRIO_BUF pData, ULONG DataBufferCount, DWORD Flags, PVOID RequestContext)
{
	STD_FUNC_ENTER();
	BOOL const rv = vtbl->RIOSend(SocketQueue, pData, DataBufferCount, Flags, RequestContext);
	if (!rv)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

BOOL call_RIOSendEx(PRIO_EXTENSION_FUNCTION_TABLE vtbl, RIO_RQ SocketQueue, PRIO_BUF pData, ULONG DataBufferCount, PRIO_BUF pLocalAddress, PRIO_BUF pRemoteAddress, PRIO_BUF pControlContext, PRIO_BUF pFlags, DWORD Flags, PVOID RequestContext)
{
	STD_FUNC_ENTER();
	BOOL const rv = vtbl->RIOSendEx(SocketQueue, pData, DataBufferCount, pLocalAddress, pRemoteAddress, pControlContext, pFlags, Flags, RequestContext);
	if (!rv)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

BOOL call_RIOReceive(PRIO_EXTENSION_FUNCTION_TABLE vtbl, RIO_RQ SocketQueue, PRIO_BUF pData, ULONG DataBufferCount, DWORD Flags, PVOID RequestContext)
{
	STD_FUNC_ENTER();
	BOOL const rv = vtbl->RIOReceive(SocketQueue, pData, DataBufferCount, Flags, RequestContext);
	if (!rv)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

RIO_RQ call_RIOCreateRequestQueue(PRIO_EXTENSION_FUNCTION_TABLE vtbl, SOCKET Socket, ULONG MaxOutstandingReceive, ULONG MaxReceiveDataBuffers, ULONG MaxOutstandingSend, ULONG MaxSendDataBuffers, RIO_CQ ReceiveCQ, RIO_CQ SendCQ, PVOID SocketContext)
{
	STD_FUNC_ENTER();
	RIO_RQ const rv = vtbl->RIOCreateRequestQueue(Socket, MaxOutstandingReceive, MaxReceiveDataBuffers, MaxOutstandingSend, MaxSendDataBuffers, ReceiveCQ, SendCQ, SocketContext);
	if (rv == RIO_INVALID_RQ)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

RIO_BUFFERID call_RIORegisterBuffer(PRIO_EXTENSION_FUNCTION_TABLE vtbl, PCHAR DataBuffer, DWORD DataLength)
{
	STD_FUNC_ENTER();
	RIO_BUFFERID const rv = vtbl->RIORegisterBuffer(DataBuffer, DataLength);
	if (rv == RIO_INVALID_BUFFERID)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

INT call_RIONotify(PRIO_EXTENSION_FUNCTION_TABLE vtbl, RIO_CQ CQ)
{
	STD_FUNC_ENTER();
	INT const rv = vtbl->RIONotify(CQ);
	if (rv != ERROR_SUCCESS)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}
#endif
