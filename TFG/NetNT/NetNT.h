#pragma once
#include <TFG/TFG.h>

namespace TFG
{

namespace NetNT
{

class Packet;
class IPacket;

class NetNT
{
public:
    NetNT(PTP_CALLBACK_ENVIRON const in_ptp_callback_environ, uint32_t const in_packet_buffer_size);
    ~NetNT();

private:
    DISALLOW_COPY_AND_ASSIGN(NetNT);

    static PTP_CALLBACK_ENVIRON ptp_callback_environ;
    static uint32_t packet_buffer_size;
	static HANDLE						iocp_h;
	static volatile int64_t			num_packets_in_use;


    friend class Packet;
};

uint32_t CopyBufferToWSABufArray(void const * const in_src_p, uint32_t const in_src_size, WSABUF const * const in_dest_wsabuf_array, uint32_t const in_dest_wsabuf_array_size, uint32_t const in_dest_offset);

} // namespace NetNT

} // namespace TFG
