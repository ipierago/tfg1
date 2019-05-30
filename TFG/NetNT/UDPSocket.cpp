#include "NetNT.h"
#include "UDPSocket.h"
#include "Packet.h"
#include "OverlappedEx.h"
#include "Globals.h"

TFG_FILE_SETUP()

namespace TFG
{

namespace NetNT
{

#pragma warning(push)
#pragma warning(disable : 4307)
TFG_TypeId const UDPSocket::TypeId = (TFG_TypeId)TFG_DJB2_16('e', 'z', 'n', 't', '_', 'u', 'd', 'p', 's', 'o', 'c', 'k', 'e', 't', '_', 't');
#pragma warning(pop)

TFG_Result UDPSocket::InvokeWSARecvFrom() const
{
	TFG_FUNC_ENTER();
	TFG_Result rv = E_FAIL;

	PacketPtr const net_packet_p = Packet::Create();
	TFG_CHECK(net_packet_p);
	TFG_CHECK(SUCCEEDED(net_packet_p->Resize(m_maxReceivePacketSize)));

	OverlappedExPtr const net_overlapped_ex_p = net_packet_p->GetOverlappedExPtr();
	net_overlapped_ex_p->object_p = (void *)this;
	net_overlapped_ex_p->_typeid = TypeId;
	net_overlapped_ex_p->context_p = net_packet_p;

	DWORD flags = 0;
	int32_t *const _p_sockaddr_in_len = net_packet_p->GetSockAddrInLenPtr();
	*_p_sockaddr_in_len = sizeof(struct sockaddr_in);
	if (m_ptpIo)
		call_StartThreadpoolIo(m_ptpIo);
	InterlockedIncrement((LPLONG)&m_numPendingOverlapped);
	uint32_t wsabuf_array_size = 0;
	WSABUF const *const wsabuf_array = net_packet_p->GetWSABufArray(&wsabuf_array_size);
	int32_t const rvWSARecvFrom = call_WSARecvFrom(m_socket, (LPWSABUF)wsabuf_array, wsabuf_array_size, NULL, &flags, (struct sockaddr *)net_packet_p->GetSockAddrInPtr(), _p_sockaddr_in_len, (LPOVERLAPPED)net_overlapped_ex_p, NULL);
	if ((rvWSARecvFrom != 0) && (!((rvWSARecvFrom == SOCKET_ERROR) && (WSAGetLastError() == WSA_IO_PENDING))))
	{
		int e = WSAGetLastError();

		if (this->m_ptpIo)
			call_CancelThreadpoolIo(this->m_ptpIo);
		InterlockedDecrement((LPLONG)& this->m_numPendingOverlapped);

		if (m_IsDeinitBegin && ((e == WSAEINTR) || (e == WSAENOTSOCK))) {
			TFG_DEBUG("WSARecvFrom returned %s after DeInit started. Socket is being closed.", TFG_WSAErrorToStringShort(e));
			TFG_GOTO_FINALLY(TFG_Level_Debug);
		} else {
			TFG_ERROR("WSARecvFrom failed with some error other than WSA_IO_PENDING: %s", TFG_WSAErrorToStringShort(e));
			TFG_GOTO_FINALLY(TFG_Level_Error);
		}
	}

	rv = S_OK;
finally:
	if (FAILED(rv))
	{
		if (net_packet_p)
			net_packet_p->Release();
	}
	TFG_FUNC_EXIT("");
	return rv;
}

void UDPSocket::OnSendComplete(PacketPtr const in_net_packet_p, TFG_Result const in_result, uint32_t const in_num_bytes_transferred)
{
	TFG_FUNC_ENTER();

	TFG_ASSERT((in_result != S_OK) || (in_num_bytes_transferred == in_net_packet_p->ComputeSize()));
	in_num_bytes_transferred;
	if (FAILED(in_result))
	{
		TFG_WARNING("Async udp send operation failed: %d: %s", in_result, TFG_GetErrorString(in_result));
	}

	in_net_packet_p->AddRef();
	m_udpSocketCallbackI->OnSendComplete(this, in_net_packet_p, in_result);

	in_net_packet_p->release_object();

	InterlockedDecrement((LPLONG)&m_numPendingOverlapped);

	TFG_FUNC_EXIT("");
}

void UDPSocket::OnRecvPacket(PacketPtr const in_net_packet_p, TFG_Result const in_result, uint32_t const in_num_bytes_transferred)
{
	TFG_FUNC_ENTER();
	TFG_Result hr = in_result;
	if (SUCCEEDED(hr))
	{
		in_net_packet_p->Resize(in_num_bytes_transferred);
		m_udpSocketCallbackI->OnRecvPacket(this, in_net_packet_p);
		hr = this->InvokeWSARecvFrom();
	}
	else
	{
		if (m_IsDeinitBegin && ((hr == __HRESULT_FROM_WIN32(ERROR_OPERATION_ABORTED)) || (hr == __HRESULT_FROM_WIN32(WSAENOTSOCK)))) {
			TFG_DEBUG("%s recv after DeInit started.  Socket is being closed.", TFG_GetErrorString(hr));
			//hr = S_OK;
		} else {
			TFG_WARNING("Async udp recv operation failed: 0x%08lx: %s", hr, TFG_GetErrorString(hr));
		}
		in_net_packet_p->Release();
	}
	if (FAILED(hr))
	{
		m_udpSocketCallbackI->OnError(this, hr);
	}

	InterlockedDecrement((LPLONG) & this->m_numPendingOverlapped);

	TFG_FUNC_EXIT("");
}

VOID CALLBACK UDPSocket::IoCompletionCallback(PTP_CALLBACK_INSTANCE in_ptp_callback_instance, PVOID in_context_pv, PVOID in_overlapped_pv, ULONG in_io_result, ULONG_PTR in_num_byte_transfered_p, PTP_IO in_ptp_io)
{
	TFG_FUNC_ENTER();
	UNREFERENCED_PARAMETER(in_context_pv);
	UNREFERENCED_PARAMETER(in_ptp_callback_instance);
	OverlappedExPtr const net_overlapped_ex_p = (OverlappedExPtr)in_overlapped_pv;

	if (net_overlapped_ex_p->_typeid == UDPSocket::TypeId)
	{
		UDPSocketPtr const in_this = (UDPSocketPtr)net_overlapped_ex_p->object_p;
		PacketPtr const net_packet_p = (PacketPtr)net_overlapped_ex_p->context_p;
		TFG_ASSERT(net_overlapped_ex_p == net_packet_p->GetOverlappedExPtr());
		TFG_ASSERT(in_ptp_io == in_this->m_ptpIo);
		in_ptp_io;
		TFG_Result const hr = HRESULT_FROM_WIN32(in_io_result);
		(*in_this).OnRecvPacket(net_packet_p, hr, in_num_byte_transfered_p);
	}
	else
	{
		TFG_ASSERT(net_overlapped_ex_p->_typeid == Packet::TypeId);
		UDPSocketPtr const in_this = (UDPSocketPtr)net_overlapped_ex_p->context_p;
		PacketPtr const net_packet_p = (PacketPtr)net_overlapped_ex_p->object_p;
		TFG_ASSERT(net_overlapped_ex_p == net_packet_p->GetOverlappedExPtr());
		TFG_ASSERT(in_ptp_io == in_this->m_ptpIo);
		TFG_Result const hr = HRESULT_FROM_WIN32(in_io_result);
		(*in_this).OnSendComplete(net_packet_p, hr, in_num_byte_transfered_p);
	}

	TFG_FUNC_EXIT("");
}

void UDPSocket::OnQueuedCompletionStatus(BOOL const in_rv_GetnQueuedCompletionStatus, uint32_t const in_num_bytes_transferred, uint32_t const in_completion_key, LPOVERLAPPED const in_overlapped_p)
{
	TFG_FUNC_ENTER();
	UNREFERENCED_PARAMETER(in_completion_key);
	UNREFERENCED_PARAMETER(in_num_bytes_transferred);
	UNREFERENCED_PARAMETER(in_rv_GetnQueuedCompletionStatus);
	OverlappedExPtr const net_overlapped_ex_p = (OverlappedExPtr)in_overlapped_p;

	DWORD num_bytes_transferred = 0;
	DWORD flags = 0;
	BOOL const rv = call_WSAGetOverlappedResult(this->m_socket, in_overlapped_p, &num_bytes_transferred, FALSE, &flags);
	TFG_Result const result = (rv ? S_OK : HRESULT_FROM_WIN32(WSAGetLastError()));

	if (net_overlapped_ex_p->_typeid == UDPSocket::TypeId)
	{
		PacketPtr const net_packet_p = (PacketPtr)net_overlapped_ex_p->context_p;
		this->OnRecvPacket(net_packet_p, result, num_bytes_transferred);
	}
	else
	{
		TFG_ASSERT(net_overlapped_ex_p->_typeid == Packet::TypeId);
		PacketPtr const net_packet_p = (PacketPtr)net_overlapped_ex_p->object_p;
		this->OnSendComplete(net_packet_p, result, num_bytes_transferred);
	}
	TFG_FUNC_EXIT("");
}

TFG_Result UDPSocket::SendPacket(PacketPtr const in_net_packet, const struct sockaddr_in *const in_sockaddr_in_p)
{
	TFG_FUNC_ENTER();
	TFG_Result rv = E_FAIL;

	OverlappedExPtr const net_overlapped_ex_p = in_net_packet->GetOverlappedExPtr();
	net_overlapped_ex_p->object_p = in_net_packet;
	net_overlapped_ex_p->_typeid = Packet::TypeId;
	net_overlapped_ex_p->context_p = this;

	in_net_packet->add_ref_object();
	if (this->m_ptpIo)
		call_StartThreadpoolIo(this->m_ptpIo);
	InterlockedIncrement((LPLONG) & this->m_numPendingOverlapped);
	uint32_t wsabuf_array_size = 0;
	WSABUF const *const wsabuf_array = in_net_packet->GetWSABufArray(&wsabuf_array_size);
	int32_t const rvWSASendTo = call_WSASendTo(this->m_socket, (LPWSABUF)wsabuf_array, wsabuf_array_size, NULL, 0, (const struct sockaddr *)in_sockaddr_in_p, sizeof(struct sockaddr_in), (LPOVERLAPPED)net_overlapped_ex_p, NULL);
	TFG_CHECK(rvWSASendTo == 0 || rvWSASendTo == SOCKET_ERROR);
	if (rvWSASendTo == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			if (this->m_ptpIo)
				call_CancelThreadpoolIo(this->m_ptpIo);
			TFG_ERROR("WSASendTo failed with some error other than WSA_IO_PENDING");
			in_net_packet->release_object();
			InterlockedDecrement((LPLONG) & this->m_numPendingOverlapped);
			TFG_GOTO_FINALLY(TFG_Level_Error);
		}
	}

	rv = S_OK;
finally:
	TFG_FUNC_EXIT("this 0x%0*" PRIxPTR ", in_net_packet 0x%0*" PRIxPTR ", return %s", PRIxPTR_WIDTH, this, PRIxPTR_WIDTH, in_net_packet, TFG_ResultToStringShort(rv));
	return rv;
}

UDPSocketPtr UDPSocket::Create(const char *const in_pszAddress, int32_t const in_Port, CallbackIPtr const in_netUDPSocketCallbackI, uint32_t const in_num_pending_receive_calls, uint32_t const in_max_receive_packet_size)
{
	TFG_FUNC_ENTER();
	UDPSocketPtr rv = 0;
	UDPSocketPtr udpSocketPtr = new UDPSocket();

	TFG_CHECK(SUCCEEDED(udpSocketPtr->Init(in_pszAddress, in_Port, in_netUDPSocketCallbackI, in_num_pending_receive_calls, in_max_receive_packet_size)));

	rv = udpSocketPtr;
finally:
	if (rv == 0)
	{
		if (udpSocketPtr)
		{
			delete udpSocketPtr;
		}
	}
	TFG_FUNC_EXIT("in_Port %d, return 0x%0*" PRIxPTR, in_Port, PRIxPTR_WIDTH, rv);
	return rv;
}

TFG_Result UDPSocket::Init(const char *const in_AddressPsz, int32_t const in_Port, CallbackIPtr const in_udpSocketCallbackI, uint32_t const in_numPendingReceiveCalls, uint32_t const in_maxReceivePacketSize)
{
	TFG_FUNC_ENTER();
	TFG_Result rv = E_FAIL;

	TFG_CHECK(in_udpSocketCallbackI);
	m_udpSocketCallbackI = in_udpSocketCallbackI;
	m_maxReceivePacketSize = in_maxReceivePacketSize;

	m_socket = call_WSASocketA(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
	TFG_CHECK(m_socket != SOCKET_ERROR);

	struct sockaddr_in _sockaddr_in;
	_sockaddr_in.sin_family = AF_INET;
	_sockaddr_in.sin_addr.s_addr = inet_addr(in_AddressPsz);
	_sockaddr_in.sin_port = htons((u_short)in_Port);
	TFG_CHECK(call_bind(m_socket, (SOCKADDR *)&_sockaddr_in, sizeof(_sockaddr_in)) != SOCKET_ERROR);

	if (g_net_globals.iocp_h)
	{
		TFG_CHECK(call_CreateIoCompletionPort((HANDLE)m_socket, g_net_globals.iocp_h, UDPSocket::TypeId, 0) == g_net_globals.iocp_h);
	}
	else
	{
		TFG_CHECK((m_ptpIo = call_CreateThreadpoolIo((HANDLE)m_socket, IoCompletionCallback, this, g_net_globals.ptp_callback_environ)) != 0);
	}

	for (uint32_t i = 0; i < in_numPendingReceiveCalls; ++i)
	{
		TFG_CHECK(SUCCEEDED(InvokeWSARecvFrom()));
	}

	g_net_globals.m_ObjectTrackerUDPSocket.Add(this);

	rv = S_OK;
finally:
	if (FAILED(rv))
		Deinit();
	TFG_FUNC_EXIT("in_Port %d, return 0x%0*" PRIxPTR, in_Port, PRIxPTR_WIDTH, rv);
	return rv;
}

void UDPSocket::Deinit()
{
	TFG_FUNC_ENTER();
	m_IsDeinitBegin = true;

	g_net_globals.m_ObjectTrackerUDPSocket.Remove(this);

	TFG_CHECK_NO_GOTO(0 == call_closesocket(this->m_socket));
	if (this->m_ptpIo)
		call_WaitForThreadpoolIoCallbacks(this->m_ptpIo, FALSE);
	if (this->m_ptpIo)
		call_CloseThreadpoolIo(this->m_ptpIo);

	while (this->m_numPendingOverlapped > 0)
	{
		Sleep(0);
	}

	TFG_FUNC_EXIT("in_this 0x%0*" PRIxPTR, PRIxPTR_WIDTH, this);
}

void UDPSocket::Destroy()
{
	Deinit();
	delete this;
}

UDPSocket::UDPSocket() : m_numPendingOverlapped(0), m_maxReceivePacketSize(0), m_socket(0), m_udpSocketCallbackI(0), m_ptpIo(0), m_IsDeinitBegin(false)
{
}
UDPSocket::~UDPSocket()
{
}

} // namespace NetNT
} // namespace TFG
