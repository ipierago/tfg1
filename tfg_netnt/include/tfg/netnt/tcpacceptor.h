#pragma once
#include "netnt.h"
#include "tcpconnection.h"
#include "overlappedex.h"

namespace TFG {

namespace NetNT {

class TCPAcceptor
{
public:
	class CallbackI
	{
	public:
		virtual void OnConnectionAttempted(TCPAcceptorPtr const in_net_tcpacceptor_p, TCPConnection::CallbackI * * const out_net_tcpconnection_callback_i_p, uint32_t * const out_receive_buffer_size_p, void * * const out_context_pv) = 0;
		virtual void OnConnectionSucceeded(TCPAcceptorPtr const in_net_tcpacceptor_p, TCPConnectionPtr const in_net_tcpconnection_p) = 0;
		virtual void OnError(TCPAcceptorPtr const in_net_tcpacceptor_p, TFG_Result const in_hr) = 0;
	protected:
		CallbackI() {}
		virtual ~CallbackI() {}
	};

	static TFG_TypeId TypeId;

	void on_queued_completion_status(BOOL const in_rv_GetnQueuedCompletionStatus, uint32_t const in_num_bytes_transferred, ULONG_PTR const in_completion_key, LPOVERLAPPED const in_overlapped_p);

	static VOID CALLBACK TCPAcceptor::io_completion_callback(PTP_CALLBACK_INSTANCE in_ptp_callback_instance, PVOID in_context_pv, PVOID in_overlapped_pv, ULONG in_io_result, ULONG_PTR in_num_byte_transfered_p, PTP_IO in_ptp_io);

	static TCPAcceptorPtr TCPAcceptor::Create(const char *const in_pszAddress, int32_t const in_Port, TCPAcceptor::CallbackI * const in_net_tcpacceptor_callback_i);
	void Destroy();

	SOCKET GetAcceptSocket() const { return accept_socket; }
	SOCKET GetSocket() const { return socket; }

private:
	TCPAcceptor::TCPAcceptor();
	TCPAcceptor::~TCPAcceptor();

	int32_t num_pending_overlapped;

	SOCKET socket;
	LPFN_ACCEPTEX accept_ex_pfn;

	CallbackI * net_tcpacceptor_callback_i;

	OverlappedEx net_overlapped_ex;

	SOCKET accept_socket;
	char output_buf[2 * (sizeof(struct sockaddr_in) + 16)];
	DWORD dwBytesReceived;

	PTP_IO ptp_io;

	TFG_Result TCPAcceptor::accept_ex();
	void TCPAcceptor::on_connection(TFG_Result const in_result);
};

}
}
