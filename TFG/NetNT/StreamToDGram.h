#pragma once
#include "NetNT.h"

namespace TFG
{
namespace NetNT
{

#pragma pack(push, 1)
// stored as network endian
typedef struct net_stream_to_dgram_header_t
{
    uint16_t size;
    uint16_t padding;
} net_stream_to_dgram_header_t;
#pragma pack(pop)
typedef net_stream_to_dgram_header_t *net_stream_to_dgram_header_p_t;
typedef net_stream_to_dgram_header_t const *net_stream_to_dgram_header_cp_t;

class StreamToDGram
{
public:
    class CallbackI
    {
    public:
        virtual void OnRecvPacket(StreamToDGram *const, Packet *const) = 0;
    };

    static StreamToDGram *StreamToDGram::Create(StreamToDGram::CallbackI *const in_CallbackIPtr);
    void StreamToDGram::Destroy();
    static PacketPtr StreamToDGram::CreateDGramPacket(PacketPtr const in_net_packet_p);

    TFG_Result StreamToDGram::OnRecvPacket(PacketPtr const in_net_packet_p);

private:
    StreamToDGram(StreamToDGram::CallbackI *const);
    ~StreamToDGram();

    CallbackI *m_CallbackIPtr;

    uint32_t header_num_bytes_read;
    net_stream_to_dgram_header_t header;
    PacketPtr dest_packet_p;
    uint32_t dest_wsabuf_index;
    uint32_t dest_index_this_buffer;
};

} // namespace NetNT

} // namespace TFG
