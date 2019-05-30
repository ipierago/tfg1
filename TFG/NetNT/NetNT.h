#pragma once
#include <TFG/TFG.h>

namespace TFG
{

namespace NetNT
{

class Singleton;
typedef class Singleton *GlobalsPtr;
class Packet;
typedef class Packet *PacketPtr;
class UDPSocket;
typedef class UDPSocket *UDPSocketPtr;
struct OverlappedEx;
typedef struct OverlappedEx *OverlappedExPtr;
class ThreadSingleton;
typedef class ThreadSingleton *ThreadSingletonPtr;
class StreamToDGram;
typedef class StreamToDGram *StreamToDGramPtr;
class TCPConnection;
typedef class TCPConnection *TCPConnectionPtr;
class TCPAcceptor;
typedef class TCPAcceptor *TCPAcceptorPtr;
class TCPConnector;
typedef class TCPConnector *TCPConnectorPtr;

TFG_Result Init(PTP_CALLBACK_ENVIRON const in_ptp_callback_environ, uint32_t const in_num_threads, uint32_t const in_packet_buffer_size);
void Deinit();
TFG_Result InitSockAddrIn(struct sockaddr_in *const in_sockaddr_in_p, const char *const in_address_p, int32_t const in_port);
uint32_t GetPacketBufferSize();
uint32_t CopyBufferToWSABufArray(void const *const in_src_p, uint32_t const in_src_size, WSABUF const *const in_dest_wsabuf_array, uint32_t const in_dest_wsabuf_array_size, uint32_t const in_dest_offset);
uint32_t CopyWSABufArrayToWSABufArray(WSABUF const *const in_src_wsabuf_array, uint32_t const in_src_wsabuf_array_size, uint32_t const in_src_offset, WSABUF const *const in_dest_wsabuf_array, uint32_t const in_dest_wsabuf_array_size, uint32_t const in_dest_offset, uint32_t const in_count);
} // namespace NetNT

} // namespace TFG

#define call_CloseHandle ::CloseHandle
#define call_CreateEventA ::CreateEventA
#define call_CreateThreadpoolIo ::CreateThreadpoolIo
#define call_StartThreadpoolIo ::StartThreadpoolIo
#define call_CancelThreadpoolIo ::CancelThreadpoolIo
#define call_WSARecv ::WSARecv
#define call_WSARecvFrom ::WSARecvFrom
#define call_WSAGetOverlappedResult ::WSAGetOverlappedResult
#define call_WSASend ::WSASend
#define call_WSASendTo ::WSASendTo
#define call_WSASocketA ::WSASocketA
#define call_bind ::bind
#define call_CreateIoCompletionPort ::CreateIoCompletionPort
#define call_closesocket ::closesocket
#define call_WaitForThreadpoolIoCallbacks ::WaitForThreadpoolIoCallbacks
#define call_CloseThreadpoolIo ::CloseThreadpoolIo
#define call_CreateThreadpool ::CreateThreadpool
#define call_GetQueuedCompletionStatus ::GetQueuedCompletionStatus
#define call_WSAStartup ::WSAStartup
#define call_CreateThread ::CreateThread
#define call_WSACleanup ::WSACleanup
#define call_PostQueuedCompletionStatus ::PostQueuedCompletionStatus
#define call_WaitForSingleObject ::WaitForSingleObject
#define call_WSAIoctl ::WSAIoctl
#define call_ConnectEx ::ConnectEx