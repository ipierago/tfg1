#pragma once
#include "NetNT.h"
#include "OverlappedEx.h"

namespace TFG
{

namespace NetNT
{

class Packet
{
public:
    int32_t AddRef();
    int32_t Release();
    OverlappedEx *GetOverlappedExPtr() { return &net_overlapped_ex; }
    OverlappedEx const *GetOverlappedExPtr() const { return &net_overlapped_ex; }
    struct sockaddr_in *GetSockAddrInPtr() { return &remote_sockaddr_in; }
    struct sockaddr_in const *GetSockAddrInPtr() const { return &remote_sockaddr_in; }
    int32_t *GetSockAddrInLenPtr() { return &remote_sockaddr_in_len; }
    int32_t const *GetSockAddrInLenPtr() const { return &remote_sockaddr_in_len; }
    WSABUF const *GetWSABufArray(uint32_t *const out_array_size) const
    {
        *out_array_size = wsabuf_vec.size();
        return (wsabuf_vec.empty() ? 0 : (WSABUF const *)&wsabuf_vec.at(0));
    }
    TFG_Result Resize(int32_t const size);
    uint32_t ComputeSize() const;
    uint32_t AppendData(void const *const in_src_p, uint32_t const in_size);
    uint32_t GetData(void *const in_dest_p, uint32_t const in_capacity);

    static Packet *Packet::Create();

    int32_t add_ref_object();
    int32_t release_object();

    static TFG_TypeId TypeId;

	friend class ThreadSingleton;

private:
    DISALLOW_COPY_AND_ASSIGN(Packet);

    Packet();
    ~Packet();

    void on_zero_interface_ref_count();
    void on_zero_object_ref_count();

    volatile int32_t object_ref_count;
    volatile int32_t interface_ref_count;
    struct sockaddr_in remote_sockaddr_in;
    int32_t remote_sockaddr_in_len;
    std::vector<WSABUF> wsabuf_vec;
    OverlappedEx net_overlapped_ex;
};

} // namespace NetNT

} // namespace TFG
