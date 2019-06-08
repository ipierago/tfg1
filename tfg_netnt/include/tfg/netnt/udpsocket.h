#pragma once
#include "netnt.h"

namespace TFG
{

namespace NetNT
{

class UDPSocket
{
public:
	class CallbackI
	{
	public:
		virtual void OnRecvPacket(UDPSocketPtr const in_net_udpsocket_p, PacketPtr const in_net_packet_p) = 0;
		virtual void OnSendComplete(UDPSocketPtr const in_net_udpsocket_p, PacketPtr const in_net_packet_p, TFG_Result const in_result) = 0;
		virtual void OnError(UDPSocketPtr const in_net_udpsocket_p, TFG_Result const in_hr) = 0;

	protected:
		CallbackI() {}
		virtual ~CallbackI() {}
	
	private:
		DISALLOW_COPY_AND_ASSIGN(CallbackI);
	};
	typedef CallbackI *CallbackIPtr;

	static const TFG_TypeId TypeId;

	static UDPSocketPtr Create(const char *const in_pszAddress, int32_t const in_Port, CallbackIPtr const in_net_udpsocket_callback_i, uint32_t const in_num_pending_receive_calls, uint32_t const in_max_receive_packet_size);
	void Destroy();
	TFG_Result SendPacket(PacketPtr const in_net_packet, const struct sockaddr_in *const in_sockaddr_in_p);

	SOCKET GetSocket() { return m_socket; }
	void OnQueuedCompletionStatus(BOOL const in_rv_GetnQueuedCompletionStatus, uint32_t const in_num_bytes_transferred, ULONG_PTR const in_completion_key, LPOVERLAPPED const in_overlapped_p);

private:
	UDPSocket();
	~UDPSocket();
	TFG_Result Init(const char *const in_AddressPsz, int32_t const in_Port, CallbackIPtr const in_udpSocketCallbackI, uint32_t const in_numPendingReceiveCalls, uint32_t const in_maxReceivePacketSize);
	void Deinit();

	int32_t m_numPendingOverlapped;
	uint32_t m_maxReceivePacketSize;

	SOCKET m_socket;

	CallbackIPtr m_udpSocketCallbackI;

	PTP_IO m_ptpIo;

	bool m_IsDeinitBegin;

	TFG_Result InvokeWSARecvFrom() const;
	void OnSendComplete(PacketPtr const in_net_packet_p, TFG_Result const in_result, uint32_t const in_num_bytes_transferred);
	void OnRecvPacket(PacketPtr const in_net_packet_p, TFG_Result const in_result, uint32_t const in_num_bytes_transferred);
	static VOID CALLBACK IoCompletionCallback(PTP_CALLBACK_INSTANCE in_ptp_callback_instance, PVOID in_context_pv, PVOID in_overlapped_pv, ULONG in_io_result, ULONG_PTR in_num_byte_transfered_p, PTP_IO in_ptp_io);
};

} // namespace NetNT
} // namespace TFG
