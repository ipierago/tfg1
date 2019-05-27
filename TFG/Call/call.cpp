#include "call_pch.h"

#include <Call/call.h>
#include <Std/Std.h>

STD_FILE_SETUP()


#ifdef WIN32

static char const * _GetLastErrorString();
#ifndef STD_DISABLE
static char const * _GetLastErrorString()
{
	return Std_GetErrorString(GetLastError());
}
#endif


SOCKET call_socket(int32_t af, int32_t type, int32_t protocol)
{
	STD_FUNC_ENTER();
	SOCKET const _socket = socket(af, type, protocol);
	if (_socket == INVALID_SOCKET)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("af %d, type %d, protocol %d, return 0x%0*" PRIxPTR, af, type, protocol, PRIxPTR_WIDTH, _socket);
	return _socket;
}

SOCKET call_accept(SOCKET s, struct sockaddr FAR * addr, int FAR * addrlen)
{
	STD_FUNC_ENTER();
	SOCKET const rv = accept(s, addr, addrlen);
	if (rv == INVALID_SOCKET)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}


int32_t call_closesocket(SOCKET in_Socket)
{
	STD_FUNC_ENTER();
	int32_t const rv = closesocket(in_Socket);
	if (rv == SOCKET_ERROR)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("in_Socket 0x%0*" PRIxPTR ", return %d", PRIxPTR_WIDTH, in_Socket, rv);
	return rv;
}

int32_t call_listen(SOCKET s, int32_t backlog)
{
	STD_FUNC_ENTER();
	int32_t const _rv = listen(s, backlog);
	if (_rv == SOCKET_ERROR)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return _rv;
}

int32_t call_bind(SOCKET in_SOCKET, const struct sockaddr FAR * in_psockaddr, int32_t in_namelen)
{
	STD_FUNC_ENTER();
	int32_t const rv = bind(in_SOCKET, in_psockaddr, in_namelen);
	if (rv == SOCKET_ERROR)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("in_SOCKET %d, in_psockaddr {sa_family %d, sa_data %s}, in_namelen %d, return %d", in_SOCKET, in_psockaddr->sa_family, in_psockaddr->sa_data, in_namelen, rv);
	return rv;
}


int call_connect(SOCKET s, const struct sockaddr *name, int namelen)
{
	STD_FUNC_ENTER();
	int rv = connect(s, name, namelen);
	if (rv == SOCKET_ERROR)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

int call_getsockname(SOCKET s, struct sockaddr *name, int *namelen)
{
	STD_FUNC_ENTER();
	int rv = getsockname(s, name, namelen);
	if (rv == SOCKET_ERROR)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

int call_sendto(SOCKET s, const char FAR * buf, int len, int flags, const struct sockaddr FAR * to, int tolen)
{
	STD_FUNC_ENTER();
	int rv = sendto(s, buf, len, flags, to, tolen);
	if (rv == SOCKET_ERROR)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

int call_send(SOCKET s, const char FAR * buf, int len, int flags)
{
	STD_FUNC_ENTER();
	int rv = send(s, buf, len, flags);
	if (rv == SOCKET_ERROR)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}


int call_recvfrom(SOCKET s, char FAR * buf, int len, int flags, struct sockaddr FAR * from, int FAR * fromlen)
{
	STD_FUNC_ENTER();
	int rv = recvfrom(s, buf, len, flags, from, fromlen);
	if (rv == SOCKET_ERROR)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

int call_recv(SOCKET s, char FAR * buf, int len, int flags)
{
	STD_FUNC_ENTER();
	int rv = recv(s, buf, len, flags);
	if (rv == SOCKET_ERROR)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

SOCKET call_WSASocketA(int32_t af, int32_t type, int32_t protocol, LPWSAPROTOCOL_INFOA lpProtocolInfo, GROUP g, DWORD dwFlags)
{
	STD_FUNC_ENTER();
	SOCKET const rv = WSASocketA(af, type, protocol, lpProtocolInfo, g, dwFlags);
	if (rv == INVALID_SOCKET)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("af %d, type %d, protocol %d, lpProtocolInfo 0x%0*" PRIxPTR ", g %d, dwFlags 0x%08lx, return 0x%0*" PRIxPTR, af, type, protocol, PRIxPTR_WIDTH, lpProtocolInfo, g, dwFlags, PRIxPTR_WIDTH, rv);
	return rv;
}

int32_t call_WSASendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr FAR * lpTo, int32_t iTolen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	STD_FUNC_ENTER();
	int32_t const rv = WSASendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iTolen, lpOverlapped, lpCompletionRoutine);
	if (rv == SOCKET_ERROR)
	{
		int32_t const rvWSAGetLastError = WSAGetLastError();
		if (rvWSAGetLastError != WSA_IO_PENDING)
		{
			STD_WARNING("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(rvWSAGetLastError));
			STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Warning);
		}
	}
	STD_FUNC_EXIT("s 0x%0*" PRIxPTR ", lpBuffers 0x%0*" PRIxPTR ", dwBufferCount %d, lpNumberOfBytesSent 0x%0*" PRIxPTR ", dwFlags 0x%08lx, lpTo 0x%0*" PRIxPTR ", iTolen %d, lpOverlapped 0x%0*" PRIxPTR ", lpCompletionRoutine 0x%0*" PRIxPTR "return %s", PRIxPTR_WIDTH, s, PRIxPTR_WIDTH, lpBuffers, dwBufferCount, PRIxPTR_WIDTH, lpNumberOfBytesSent, dwFlags, PRIxPTR_WIDTH, lpTo, iTolen, PRIxPTR_WIDTH, lpOverlapped, PRIxPTR_WIDTH, lpCompletionRoutine, STD_BOOL_TO_STRING(rv));
	return rv;
}

int32_t call_WSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	STD_FUNC_ENTER();
	int32_t const _rv = WSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
	if (_rv == SOCKET_ERROR)
	{
		int32_t const rvWSAGetLastError = WSAGetLastError();
		if (rvWSAGetLastError != WSA_IO_PENDING)
		{
			STD_WARNING("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(rvWSAGetLastError));
			STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Warning);
		}
	}
	STD_FUNC_EXIT("");
	return _rv;
}

int32_t call_WSARecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, struct sockaddr FAR * lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	STD_FUNC_ENTER();
	int32_t const rv = WSARecvFrom(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpFrom, lpFromlen, lpOverlapped, lpCompletionRoutine);
	if (rv == SOCKET_ERROR)
	{
		int32_t const rvWSAGetLastError = WSAGetLastError();
		if (rvWSAGetLastError != WSA_IO_PENDING)
		{
			STD_WARNING("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(rvWSAGetLastError));
			STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Warning);
		}
	}
	STD_FUNC_EXIT("s 0x%0*" PRIxPTR ", lpBuffers 0x%0*" PRIxPTR ", dwBufferCount %d, lpNumberOfBytesRecvd 0x%0*" PRIxPTR ", lpFlags 0x%0*" PRIxPTR ", lpFrom 0x%0*" PRIxPTR ", lpFromlen 0x%0*" PRIxPTR ", lpOverlapped 0x%0*" PRIxPTR ", lpCompletionRoutine 0x%0*" PRIxPTR " return %s", PRIxPTR_WIDTH, s, PRIxPTR_WIDTH, lpBuffers, dwBufferCount, PRIxPTR_WIDTH, lpNumberOfBytesRecvd, PRIxPTR_WIDTH, lpFlags, PRIxPTR_WIDTH, lpFrom, PRIxPTR_WIDTH, lpFromlen, PRIxPTR_WIDTH, lpOverlapped, PRIxPTR_WIDTH, lpCompletionRoutine, STD_BOOL_TO_STRING(rv));
	return rv;
}

int32_t call_WSARecv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	STD_FUNC_ENTER();
	int32_t const _rv = WSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
	if (_rv == SOCKET_ERROR)
	{
		int32_t const rvWSAGetLastError = WSAGetLastError();
		if (rvWSAGetLastError != WSA_IO_PENDING)
		{
			STD_WARNING("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(rvWSAGetLastError));
			STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Warning);
		}
	}
	STD_FUNC_EXIT("");
	return _rv;
}


BOOL call_ConnectEx(LPFN_CONNECTEX const in_pfn_ConnectEx, SOCKET s, const struct sockaddr *name, int32_t namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped)
{
	STD_FUNC_ENTER();
	BOOL const _rv = in_pfn_ConnectEx(s, name, namelen, lpSendBuffer, dwSendDataLength, lpdwBytesSent, lpOverlapped);
	if (_rv == FALSE)
	{
		int32_t const rvWSAGetLastError = WSAGetLastError();
		if (rvWSAGetLastError != WSA_IO_PENDING)
		{
			STD_WARNING("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(rvWSAGetLastError));
			STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Warning);
		}
	}
	STD_FUNC_EXIT("");
	return _rv;
}

BOOL call_AcceptEx(LPFN_ACCEPTEX const in_pfn_AcceptEx, SOCKET sListenSocket, SOCKET sAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped)
{
	STD_FUNC_ENTER();
	BOOL const _rv = in_pfn_AcceptEx(sListenSocket, sAcceptSocket, lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, lpdwBytesReceived, lpOverlapped);
	if (_rv == FALSE)
	{
		int32_t const rvWSAGetLastError = WSAGetLastError();
		if (rvWSAGetLastError != WSA_IO_PENDING)
		{
		STD_WARNING("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Warning);
		}
	}
	STD_FUNC_EXIT("");
	return _rv;
}

int32_t call_WSAIoctl(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer, DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	STD_FUNC_ENTER();
	int32_t const _rv = WSAIoctl(s, dwIoControlCode, lpvInBuffer, cbInBuffer, lpvOutBuffer, cbOutBuffer, lpcbBytesReturned, lpOverlapped, lpCompletionRoutine);
	if (_rv == SOCKET_ERROR)
	{
		STD_ERROR("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return _rv;
}

#endif


#ifdef WIN32

BOOL call_WSAGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags)
{
	STD_FUNC_ENTER();
	BOOL const rv = WSAGetOverlappedResult(s, lpOverlapped, lpcbTransfer, fWait, lpdwFlags);
	if (rv == FALSE)
	{
		STD_WARNING("WSAGetLastError() returns %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Warning);

	}
	STD_FUNC_EXIT("s 0x%0*" PRIxPTR ", lpOverlapped 0x%0*" PRIxPTR " lpcbTransfer 0x%0*" PRIxPTR ", fWait %s, lpdwFlags 0x%0*" PRIxPTR ", return %s, *lpcbTransfer %d, *lpdwFlags 0x%08lx", PRIxPTR_WIDTH, s, PRIxPTR_WIDTH, lpOverlapped, PRIxPTR_WIDTH, lpcbTransfer, STD_BOOL_TO_STRING(fWait), PRIxPTR_WIDTH, lpdwFlags, STD_BOOL_TO_STRING(rv), (lpcbTransfer ? *lpcbTransfer : 0), (lpdwFlags ? *lpdwFlags : 0));
	return rv;
}


HANDLE call_CreateEventA(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCSTR lpName)
{
	STD_FUNC_ENTER();
	HANDLE const rv = CreateEventA(lpEventAttributes, bManualReset, bInitialState, lpName);
	if (rv == NULL)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("lpEventAttributes 0x%0*" PRIxPTR ", bManualReset %s, bInitialState %s, lpName %s, return 0x%0*" PRIxPTR, PRIxPTR_WIDTH, lpEventAttributes, STD_BOOL_TO_STRING(bManualReset), STD_BOOL_TO_STRING(bInitialState), lpName, PRIxPTR_WIDTH, rv);
	return rv;
}

BOOL call_CloseHandle(HANDLE hObject)
{
	STD_FUNC_ENTER();
	BOOL const rv = CloseHandle(hObject);
	if (rv == 0)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("hObject 0x%0*" PRIxPTR " return %s", PRIxPTR_WIDTH, hObject, STD_BOOL_TO_STRING(rv));
	return rv;
}

HANDLE call_CreateThread(LPSECURITY_ATTRIBUTES  lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId)
{
	STD_FUNC_ENTER();
	HANDLE const rv = CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
	if (rv == NULL)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("lpThreadAttributes 0x%0*" PRIxPTR ", dwStackSize 0x%0*" PRIxPTR ", lpStartAddress 0x%0*" PRIxPTR ", lpParameter 0x%0*" PRIxPTR ", dwCreationFlags 0x%08lx return 0x%0*" PRIxPTR ", *lpThreadId 0x%08lx", PRIxPTR_WIDTH, lpThreadAttributes, PRIxPTR_WIDTH, dwStackSize, PRIxPTR_WIDTH, lpStartAddress, PRIxPTR_WIDTH, lpParameter, dwCreationFlags, PRIxPTR_WIDTH, rv, (lpThreadId ? *lpThreadId : 0));
	return rv;
}

BOOL call_TerminateThread(HANDLE hThread, DWORD dwExitCode)
{
	STD_FUNC_ENTER();
	BOOL const rv = TerminateThread(hThread, dwExitCode);
	if (!rv)
	{
		STD_ERROR("GetLastError() returns 0x%08lx : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("hThread 0x%0*" PRIxPTR ", dwExitCode %d, return %s", PRIxPTR_WIDTH, hThread, dwExitCode, STD_BOOL_TO_STRING(rv));
	return rv;
}

BOOL call_SetEvent(HANDLE hEvent)
{
	STD_FUNC_ENTER();
	BOOL const rv = SetEvent(hEvent);
	if (rv == 0)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("hEvent 0x%0*" PRIxPTR " return %s", PRIxPTR_WIDTH, hEvent, STD_BOOL_TO_STRING(rv));
	return rv;
}

DWORD call_WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds)
{
	STD_FUNC_ENTER();
	DWORD const rv = WaitForSingleObject(hHandle, dwMilliseconds);
	if (rv == WAIT_FAILED)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("hHandle 0x%0*" PRIxPTR ", dwMilliseconds %d, return %d", PRIxPTR_WIDTH, hHandle, dwMilliseconds, rv);
	return rv;
}

DWORD call_WaitForMultipleObjects(DWORD nCount, CONST HANDLE *lpHandles, BOOL bWaitAll, DWORD dwMilliseconds)
{
	STD_FUNC_ENTER();
	DWORD const rv = WaitForMultipleObjects(nCount, lpHandles, bWaitAll, dwMilliseconds);
	if (rv == WAIT_FAILED)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("nCount %d, lpHandles 0x%0*" PRIxPTR ", bWaitAll %s, dwMilliseconds %d", nCount, PRIxPTR_WIDTH, lpHandles, STD_BOOL_TO_STRING(bWaitAll), dwMilliseconds);
	return rv;
}


void call_DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	STD_FUNC_ENTER();
	DeleteCriticalSection(lpCriticalSection);
	STD_FUNC_EXIT("lpCriticalSection 0x%0*" PRIxPTR, PRIxPTR_WIDTH, lpCriticalSection);
}


void call_InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	STD_FUNC_ENTER();
	InitializeCriticalSection(lpCriticalSection);
	STD_FUNC_EXIT("lpCriticalSection 0x%0*" PRIxPTR, PRIxPTR_WIDTH, lpCriticalSection);
}

void call_EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	STD_FUNC_ENTER();
	EnterCriticalSection(lpCriticalSection);
	STD_FUNC_EXIT("lpCriticalSection 0x%0*" PRIxPTR, PRIxPTR_WIDTH, lpCriticalSection);
}

void call_LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	STD_FUNC_ENTER();
	LeaveCriticalSection(lpCriticalSection);
	STD_FUNC_EXIT("lpCriticalSection 0x%0*" PRIxPTR, PRIxPTR_WIDTH, lpCriticalSection);
}
#endif

LPVOID call_malloc(int32_t const in_Size)
{
	STD_FUNC_ENTER();
	LPVOID const pv = malloc(in_Size);
	if (pv == NULL)
	{
		STD_ERROR("errno() return %d", errno);
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}

	STD_FUNC_EXIT("in_Size %d, return 0x%0*" PRIxPTR, in_Size, PRIxPTR_WIDTH, pv);
	return pv;
}

void call_free(void * const in_pv)
{
	STD_FUNC_ENTER();
	free(in_pv);
	STD_FUNC_EXIT("in_pv 0x%0*" PRIxPTR, PRIxPTR_WIDTH, in_pv);
}

void * call_memset(void *dest, int32_t c, size_t count)
{
	STD_FUNC_ENTER();
	void * rv = memset(dest, c, count);
	STD_FUNC_EXIT("dest 0x%0*" PRIxPTR ", c %d, count %d, return 0x%0*" PRIxPTR, PRIxPTR_WIDTH, dest, c, count, PRIxPTR_WIDTH, rv);
	return rv;
}

void* call_memcpy(void* _Dst, void const* _Src, size_t _Size)
{
	STD_FUNC_ENTER();
	void * rv = memcpy(_Dst, _Src, _Size);
	STD_FUNC_EXIT("_Dst 0x%0*" PRIxPTR ", _Src 0x%0*" PRIxPTR ", _Size %d, return 0x%0*" PRIxPTR, PRIxPTR_WIDTH, _Dst, PRIxPTR_WIDTH, _Src, _Size, PRIxPTR_WIDTH, rv);
	return rv;
}

#ifndef WIN32
static errno_t strcpy_s(char* _Destination, rsize_t, char const* _Source)
{
    strcpy(_Destination, _Source);
    return 0;
}
#endif

errno_t call_strcpy_s(char* _Destination, rsize_t _SizeInBytes, char const* _Source)
{
	STD_FUNC_ENTER();
	errno_t const rv = strcpy_s(_Destination, _SizeInBytes, _Source);
	if (rv != 0) STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	STD_FUNC_EXIT("_Destination 0x%0*" PRIxPTR ", _SizeInBytes %d, Source 0x%0*" PRIxPTR ", return %d", PRIxPTR_WIDTH, _Destination, _SizeInBytes, PRIxPTR_WIDTH, _Source, rv);
	return rv;
}

#ifdef WIN32

int32_t call_WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
{
	STD_FUNC_ENTER();
	int32_t rv= WSAStartup(wVersionRequested, lpWSAData);
	if (rv != 0)
	{
		STD_ERROR("WSAGetLastError() returned %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("wVersionRequest %d, return %s, lpWSAData {wVersion %d, wHighVersion %d, szDescription %s, szSystemStatus, iMaxSockets %d, iMaxUdpDg %d, lpVendorInfo %s}", wVersionRequested, Std_WSAErrorToStringShort(rv), lpWSAData->wVersion, lpWSAData->wHighVersion, lpWSAData->szDescription, lpWSAData->szSystemStatus, lpWSAData->iMaxSockets, lpWSAData->iMaxUdpDg, lpWSAData->lpVendorInfo);
	return rv;
}

int32_t call_WSACleanup()
{
	STD_FUNC_ENTER();
	int32_t rv = WSACleanup();
	if (rv == SOCKET_ERROR)
	{
		STD_ERROR("WSAGetLastError() returned %s", Std_WSAErrorToStringShort(WSAGetLastError()));
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("return %d", rv);
	return rv;
}

HANDLE call_CreateIoCompletionPort(HANDLE FileHandle, HANDLE ExistingCompletionPort, ULONG_PTR CompletionKey, DWORD NumberOfConcurrentThreads)
{
	STD_FUNC_ENTER();
	HANDLE const rv = CreateIoCompletionPort(FileHandle, ExistingCompletionPort, CompletionKey, NumberOfConcurrentThreads);
	if (rv == NULL)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

BOOL call_GetQueuedCompletionStatus(HANDLE CompletionPort, LPDWORD lpNumberOfBytesTransferred, PULONG_PTR lpCompletionKey, LPOVERLAPPED * lpOverlapped, DWORD dwMilliseconds)
{
	STD_FUNC_ENTER();
	BOOL const rv = GetQueuedCompletionStatus(CompletionPort, lpNumberOfBytesTransferred, lpCompletionKey, lpOverlapped, dwMilliseconds);
	if (rv == FALSE)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

BOOL call_PostQueuedCompletionStatus(HANDLE CompletionPort, DWORD dwNumberOfBytesTransferred, ULONG_PTR dwCompletionKey, LPOVERLAPPED lpOverlapped)
{
	STD_FUNC_ENTER();
	BOOL const rv = PostQueuedCompletionStatus(CompletionPort, dwNumberOfBytesTransferred, dwCompletionKey, lpOverlapped);
	if (rv == FALSE)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

VOID call_DestroyThreadpoolEnvironment(PTP_CALLBACK_ENVIRON pcbe)
{
	STD_FUNC_ENTER();
	DestroyThreadpoolEnvironment(pcbe);
	STD_FUNC_EXIT("pcbe 0x0*" PRIxPTR, PRIxPTR_WIDTH, pcbe);
}

PTP_IO call_CreateThreadpoolIo(HANDLE fl, PTP_WIN32_IO_CALLBACK pfnio, PVOID pv, PTP_CALLBACK_ENVIRON pcbe)
{
	STD_FUNC_ENTER();
	PTP_IO const rv = CreateThreadpoolIo(fl, pfnio, pv, pcbe);
	if (rv == 0)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("fl 0x%0*" PRIxPTR ", pfnio 0x%0*" PRIxPTR ", pv 0x%0*" PRIxPTR ", pcbe 0x%0*" PRIxPTR ", return 0x%0*" PRIxPTR, PRIxPTR_WIDTH, fl, PRIxPTR_WIDTH, pfnio, PRIxPTR_WIDTH, pv, PRIxPTR_WIDTH, pcbe, PRIxPTR_WIDTH, rv);
	return rv;
}

VOID call_CloseThreadpoolIo(PTP_IO pio)
{
	STD_FUNC_ENTER();
	CloseThreadpoolIo(pio);
	STD_FUNC_EXIT("pio 0x%0*" PRIxPTR, PRIxPTR_WIDTH, pio);
}

VOID call_StartThreadpoolIo(PTP_IO pio)
{
	STD_FUNC_ENTER();
	StartThreadpoolIo(pio);
	STD_FUNC_EXIT("pio 0x%0*" PRIxPTR, PRIxPTR_WIDTH, pio);
}

VOID call_CancelThreadpoolIo(PTP_IO pio)
{
	STD_FUNC_ENTER();
	CancelThreadpoolIo(pio);
	STD_FUNC_EXIT("");
}

VOID call_WaitForThreadpoolIoCallbacks(PTP_IO pio, BOOL fCancelPendingCallbacks)
{
	STD_FUNC_ENTER();
	WaitForThreadpoolIoCallbacks(pio, fCancelPendingCallbacks);
	STD_FUNC_EXIT("");
}

PTP_WORK call_CreateThreadpoolWork(PTP_WORK_CALLBACK pfnwk, PVOID pv, PTP_CALLBACK_ENVIRON pcbe)
{
	STD_FUNC_ENTER();
	PTP_WORK const rv = CreateThreadpoolWork(pfnwk, pv, pcbe);
	if (rv == 0)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("pfnwk 0x%0*" PRIxPTR ", pv 0x%0*" PRIxPTR ", pcbe 0x%0*" PRIxPTR ", return 0x%0*" PRIxPTR, PRIxPTR_WIDTH, pfnwk, PRIxPTR_WIDTH, pv, PRIxPTR_WIDTH, pcbe, PRIxPTR_WIDTH, rv);
	return rv;
}

VOID call_WaitForThreadpoolWorkCallbacks(PTP_WORK pwk, BOOL fCancelPendingCallbacks)
{
	STD_FUNC_ENTER();
	WaitForThreadpoolWorkCallbacks(pwk, fCancelPendingCallbacks);
	STD_FUNC_EXIT("");
}

VOID call_CloseThreadpoolWork(PTP_WORK pwk)
{
	STD_FUNC_ENTER();
	CloseThreadpoolWork(pwk);
	STD_FUNC_EXIT("");
}

VOID call_SubmitThreadpoolWork(PTP_WORK pwk)
{
	STD_FUNC_ENTER();
	SubmitThreadpoolWork(pwk);
	STD_FUNC_EXIT("");
}

VOID call_InitializeThreadpoolEnvironment(PTP_CALLBACK_ENVIRON pcbe)
{
	STD_FUNC_ENTER();
	InitializeThreadpoolEnvironment(pcbe);
	STD_FUNC_EXIT("");
}

PTP_POOL call_CreateThreadpool(PVOID reserved)
{
	STD_FUNC_ENTER();
	PTP_POOL const rv = CreateThreadpool(reserved);
	if (rv == 0)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

PTP_TIMER call_CreateThreadpoolTimer(PTP_TIMER_CALLBACK pfnti, PVOID pv, PTP_CALLBACK_ENVIRON pcbe)
{
	STD_FUNC_ENTER();
	PTP_TIMER const rv = CreateThreadpoolTimer(pfnti, pv, pcbe);
	if (rv == 0)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

BOOL call_SetConsoleCtrlHandler(PHANDLER_ROUTINE HandlerRoutine, BOOL Add)
{
	STD_FUNC_ENTER();
	BOOL rv = SetConsoleCtrlHandler(HandlerRoutine, Add);
	if (rv == 0)
	{
		STD_ERROR("GetLastError() returns %d : %s", GetLastError(), _GetLastErrorString());
		STD_SET_THIS_LEVEL_FUNC_EXIT(StdLevel_Error);
	}
	STD_FUNC_EXIT("");
	return rv;
}

#endif
