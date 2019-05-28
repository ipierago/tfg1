#pragma once
#include <TFG/TFG.h>

namespace TFG
{

namespace NetNT
{

class Globals;
typedef class Globals *GlobalsPtr;
class Packet;
typedef class Packet *PacketPtr;
class UDPSocket;
typedef class UDPSocket *UDPSocketPtr;
struct OverlappedEx;
typedef struct OverlappedEx *OverlappedExPtr;

TFG_Result						NetNT_Init									(PTP_CALLBACK_ENVIRON const in_ptp_callback_environ, uint32_t const in_num_threads, uint32_t const in_packet_buffer_size);
void							NetNT_Deinit								();
uint32_t						NetNT_GetPacketBufferSize					();


uint32_t NetNT_CopyBufferToWSABufArray(void const *const in_src_p, uint32_t const in_src_size, WSABUF const *const in_dest_wsabuf_array, uint32_t const in_dest_wsabuf_array_size, uint32_t const in_dest_offset);

} // namespace NetNT

} // namespace TFG

#define call_CloseHandle ::CloseHandle
#define call_CreateEventA ::CreateEventA
#define call_CreateThreadpoolIo ::CreateThreadpoolIo
#define call_StartThreadpoolIo ::StartThreadpoolIo
#define call_CancelThreadpoolIo ::CancelThreadpoolIo
#define call_WSARecvFrom ::WSARecvFrom
#define call_WSAGetOverlappedResult ::WSAGetOverlappedResult
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