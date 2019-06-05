#pragma once

#include "NetNT.h"

namespace TFG
{
namespace NetNT
{

class TCPConnection
{
public:
    class CallbackI
    {
    public:
        virtual void OnRecvPacket(TCPConnectionPtr const in_net_tcpconnection_p, PacketPtr const in_net_packet_p) = 0;
        virtual void OnSendComplete(TCPConnectionPtr const in_net_tcpconnection_p, PacketPtr const in_net_packet_p, TFG_Result const in_result) = 0;
        virtual void OnError(TCPConnectionPtr const in_net_tcpconnection_p, TFG_Result const in_hr) = 0;

    protected:
		CallbackI() {}
        virtual ~CallbackI() {}
	};

    SOCKET GetSocket()
    {
        return socket;
    }
    void *GetContext() { return context_pv; }

    static TCPConnectionPtr Create(SOCKET const in_socket, PTP_IO const in_ptp_io, CallbackI *const in_net_tcpconnection_callback_i, uint32_t const in_receive_buffer_size, void *const in_context_pv);
	void Destroy();

    TFG_Result TCPConnection::SendPacket(PacketPtr const in_net_packet);

    static VOID CALLBACK io_completion_callback(PTP_CALLBACK_INSTANCE in_ptp_callback_instance, PVOID in_context_pv, PVOID in_overlapped_pv, ULONG in_io_result, ULONG_PTR in_num_byte_transfered_p, PTP_IO in_ptp_io);

    static TFG_TypeId TypeId;

    void on_queued_completion_status(BOOL const in_rv_GetnQueuedCompletionStatus, uint32_t const in_num_bytes_transferred, ULONG_PTR const in_completion_key, LPOVERLAPPED const in_overlapped_p);

private:
    TCPConnection();
    ~TCPConnection();

    TFG_Result TCPConnection::CallWSARecv();
    void TCPConnection::on_send_complete(PacketPtr const in_net_packet_p, TFG_Result const in_result, uint32_t const in_num_bytes_transferred);
    void TCPConnection::on_recv_packet(PacketPtr const in_net_packet_p, TFG_Result const in_result, uint32_t const in_num_bytes_transferred);

    int32_t num_pending_overlapped;
    uint32_t receive_buffer_size;
	CallbackI * m_CallbackIPtr;
    SOCKET socket;
    PTP_IO ptp_io;
    void *context_pv;
};

} // namespace NetNT
} // namespace TFG