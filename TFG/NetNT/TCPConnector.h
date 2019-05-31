#pragma once
#include "NetNT.h"
#include "TCPConnection.h"
#include "OverlappedEx.h"

namespace TFG
{
namespace NetNT
{

class TCPConnector
{
public:
    class CallbackI
    {
    public:
        virtual void OnConnectionAttempted(TCPConnectorPtr const in_net_tcpconnector_p, TCPConnection::CallbackI * *const out_net_tcpconnection_callback_i_p, uint32_t *const out_receive_buffer_size_p, void **const out_context_pv) = 0;
		virtual void OnConnectionSucceeded(TCPConnectorPtr const in_net_tcpconnector_p, TCPConnectionPtr const in_net_tcpconnection_p) = 0;
		virtual void OnError(TCPConnectorPtr const in_net_tcpconnector_p, TFG_Result const in_hr) = 0;

    protected:
        CallbackI() {}
        virtual ~CallbackI() {}
    };

	static VOID CALLBACK io_completion_callback(PTP_CALLBACK_INSTANCE in_ptp_callback_instance, PVOID in_context_pv, PVOID in_overlapped_pv, ULONG in_io_result, ULONG_PTR in_num_byte_transfered_p, PTP_IO in_ptp_io);

	static TFG_TypeId TypeId;

	void on_queued_completion_status(BOOL const in_rv_GetnQueuedCompletionStatus, uint32_t const in_num_bytes_transferred, uint32_t const in_completion_key, LPOVERLAPPED const in_overlapped_p);

	static TCPConnectorPtr TCPConnector::Create(const char *const in_pszAddressLocal, int32_t const in_PortLocal, const char *const in_pszAddressRemote, int32_t const in_PortRemote, TCPConnector::CallbackI * const in_net_tcpconnector_callback_i);
	void Destroy();

private:
	TCPConnector();
	~TCPConnector();
    int32_t num_pending_overlapped;
    SOCKET socket;
    CallbackI *net_tcpconnector_callback_i;
    OverlappedEx net_overlapped_ex;
    PTP_IO ptp_io;

	void on_connection(TFG_Result const in_result);
};

} // namespace NetNT
} // namespace TFG

