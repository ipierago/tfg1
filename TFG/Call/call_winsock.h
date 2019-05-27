#pragma once

#include <Call/call_fwd.h>
#include <Sys/Sys.h>

#ifdef WIN32
CALL_API SOCKET call_socket(int32_t af, int32_t type, int32_t protocol);
CALL_API int32_t call_closesocket(SOCKET in_Socket);
CALL_API int32_t call_bind(SOCKET in_SOCKET, const struct sockaddr FAR * in_psockaddr, int32_t in_namelen);
CALL_API SOCKET call_accept( SOCKET s, struct sockaddr FAR * addr, int FAR * addrlen );
CALL_API int call_send(SOCKET s, const char FAR * buf, int len, int flags);
CALL_API int call_sendto(SOCKET s, const char FAR * buf, int len, int flags, const struct sockaddr FAR * to, int tolen);
CALL_API int call_recv(SOCKET s, char FAR * buf, int len, int flags);
CALL_API int call_recvfrom(SOCKET s, char FAR * buf, int len, int flags, struct sockaddr FAR * from, int FAR * fromlen);
CALL_API int32_t call_listen(SOCKET s, int32_t backlog);
CALL_API int call_connect(SOCKET s, const struct sockaddr *name, int namelen);
CALL_API int call_getsockname(SOCKET s, struct sockaddr *name, int *namelen);
CALL_API int32_t call_WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
CALL_API int32_t call_WSACleanup();
CALL_API BOOL call_WSAGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait, LPDWORD lpdwFlags);
CALL_API int32_t call_WSARecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, struct sockaddr FAR * lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
CALL_API int32_t call_WSARecv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
CALL_API int32_t call_WSASendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr FAR * lpTo, int32_t iTolen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
CALL_API int32_t call_WSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
CALL_API SOCKET call_WSASocketA(int32_t af, int32_t type, int32_t protocol, LPWSAPROTOCOL_INFOA lpProtocolInfo, GROUP g, DWORD dwFlags);
CALL_API BOOL call_ConnectEx(LPFN_CONNECTEX const in_pfn_ConnectEx, SOCKET s, const struct sockaddr *name, int32_t namelen, PVOID lpSendBuffer, DWORD dwSendDataLength, LPDWORD lpdwBytesSent, LPOVERLAPPED lpOverlapped);
CALL_API BOOL call_AcceptEx(LPFN_ACCEPTEX const in_pfn_AcceptEx, SOCKET sListenSocket, SOCKET sAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped);
CALL_API int32_t call_WSAIoctl(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer, DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
#endif
