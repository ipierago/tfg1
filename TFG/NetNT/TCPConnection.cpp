#include "NetNT.h"
#include "TCPConnection.h"

#include "ObjectTracker.h"
#include "Singleton.h"
#include "Packet.h"
#include "OverlappedEx.h"

TFG_FILE_SETUP()

namespace TFG
{
namespace NetNT
{

#pragma warning(push)
#pragma warning(disable : 4307)
TFG_TypeId TCPConnection::TypeId = TFG_DJB2_20('e', 'z', 'n', 't', '_', 't', 'c', 'p', 'c', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'o', 'n', '_', 't');
#pragma warning(pop)

TFG_Result TCPConnection::CallWSARecv()
{
	TFG_FUNC_ENTER();
	TFG_Result rv = E_FAIL;

	PacketPtr const net_packet_p = Packet::Create();
	TFG_CHECK(net_packet_p);
	TFG_CHECK(SUCCEEDED(net_packet_p->Resize(receive_buffer_size)));

	OverlappedExPtr const net_overlapped_ex_p = net_packet_p->GetOverlappedExPtr();
	net_overlapped_ex_p->object_p = this;
	net_overlapped_ex_p->_typeid = TCPConnection::TypeId;
	net_overlapped_ex_p->context_p = net_packet_p;

	DWORD flags = 0;
	if (ptp_io)
		call_StartThreadpoolIo(ptp_io);
	InterlockedIncrement((LPLONG) & num_pending_overlapped);
	uint32_t wsabuf_array_size = 0;
	WSABUF const *const wsabuf_array = net_packet_p->GetWSABufArray(&wsabuf_array_size);
	int32_t const rvWSARecv = call_WSARecv(socket, (LPWSABUF)wsabuf_array, wsabuf_array_size, NULL, &flags, (LPOVERLAPPED)net_overlapped_ex_p, NULL);
	if ((rvWSARecv != 0) && (!((rvWSARecv == SOCKET_ERROR) && (WSAGetLastError() == WSA_IO_PENDING))))
	{
		TFG_ERROR("WSARecv failed with some error other than WSA_IO_PENDING");
		if (ptp_io)
			call_CancelThreadpoolIo(ptp_io);
		InterlockedDecrement((LPLONG) & num_pending_overlapped);
		TFG_GOTO_FINALLY(TFG_Level_Error);
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

void TCPConnection::on_send_complete(PacketPtr const in_net_packet_p, TFG_Result const in_result, uint32_t const in_num_bytes_transferred)
{
	TFG_FUNC_ENTER();
	TFG_ASSERT(FAILED(in_result) || (in_num_bytes_transferred == in_net_packet_p->ComputeSize()));
	in_num_bytes_transferred;
	if (FAILED(in_result))
	{
		TFG_WARNING("Async tcp send operation failed: %d: %s", in_result, TFG_GetErrorString(in_result));
	}

	in_net_packet_p->AddRef();
	m_CallbackIPtr->OnSendComplete(this, in_net_packet_p, in_result);
	in_net_packet_p->release_object();

	InterlockedDecrement((LPLONG) & num_pending_overlapped);

	TFG_FUNC_EXIT("");
}

void TCPConnection::on_recv_packet(PacketPtr const in_net_packet_p, TFG_Result const in_result, uint32_t const in_num_bytes_transferred)
{
	TFG_FUNC_ENTER();
	TFG_Result hr = in_result;
	if (SUCCEEDED(hr))
	{
		in_net_packet_p->Resize(in_num_bytes_transferred);
		m_CallbackIPtr->OnRecvPacket(this, in_net_packet_p);
		hr = CallWSARecv();
	}
	else
	{
		TFG_WARNING("Async tcp recv operation failed: %d: %s", hr, TFG_GetErrorString(hr));
		in_net_packet_p->Release();
	}
	if (FAILED(hr))
	{
		m_CallbackIPtr->OnError(this, hr);
	}

	InterlockedDecrement((LPLONG) & num_pending_overlapped);

	TFG_FUNC_EXIT("");
}

VOID CALLBACK TCPConnection::io_completion_callback(PTP_CALLBACK_INSTANCE in_ptp_callback_instance, PVOID in_context_pv, PVOID in_overlapped_pv, ULONG in_io_result, ULONG_PTR in_num_byte_transfered_p, PTP_IO in_ptp_io)
{
	TFG_FUNC_ENTER();
	UNREFERENCED_PARAMETER(in_context_pv);
	UNREFERENCED_PARAMETER(in_ptp_callback_instance);
	OverlappedExPtr const net_overlapped_ex_p = (OverlappedExPtr)in_overlapped_pv;

	if (net_overlapped_ex_p->_typeid == TCPConnection::TypeId)
	{
		TCPConnectionPtr const tcpConnectionPtr = (TCPConnectionPtr)net_overlapped_ex_p->object_p;
		PacketPtr const net_packet_p = (PacketPtr)net_overlapped_ex_p->context_p;
		TFG_ASSERT(net_overlapped_ex_p == net_packet_p->GetOverlappedExPtr());
		TFG_ASSERT(in_ptp_io == tcpConnectionPtr->ptp_io);
		in_ptp_io;
		TFG_Result const hr = HRESULT_FROM_WIN32(in_io_result);
		tcpConnectionPtr->on_recv_packet(net_packet_p, hr, in_num_byte_transfered_p);
	}
	else
	{
		TFG_ASSERT(net_overlapped_ex_p->_typeid == Packet::TypeId);
		TCPConnectionPtr const tcpConnectionPtr = (TCPConnectionPtr)net_overlapped_ex_p->context_p;
		PacketPtr const net_packet_p = (PacketPtr)net_overlapped_ex_p->object_p;
		TFG_ASSERT(net_overlapped_ex_p == net_packet_p->GetOverlappedExPtr());
		TFG_ASSERT(in_ptp_io == tcpConnectionPtr->ptp_io);
		TFG_Result const hr = HRESULT_FROM_WIN32(in_io_result);
		tcpConnectionPtr->on_send_complete(net_packet_p, hr, in_num_byte_transfered_p);
	}

	TFG_FUNC_EXIT("");
}

void TCPConnection::on_queued_completion_status(BOOL const in_rv_GetnQueuedCompletionStatus, uint32_t const in_num_bytes_transferred, uint32_t const in_completion_key, LPOVERLAPPED const in_overlapped_p)
{
	TFG_FUNC_ENTER();
	UNREFERENCED_PARAMETER(in_completion_key);
	UNREFERENCED_PARAMETER(in_num_bytes_transferred);
	UNREFERENCED_PARAMETER(in_rv_GetnQueuedCompletionStatus);
	OverlappedExPtr const net_overlapped_ex_p = (OverlappedExPtr)in_overlapped_p;

	DWORD num_bytes_transferred = 0;
	DWORD flags = 0;
	BOOL const rv = call_WSAGetOverlappedResult(socket, in_overlapped_p, &num_bytes_transferred, FALSE, &flags);
	TFG_Result const result = (rv ? S_OK : HRESULT_FROM_WIN32(WSAGetLastError()));

	if (net_overlapped_ex_p->_typeid == TCPConnection::TypeId)
	{
		PacketPtr const net_packet_p = (PacketPtr)net_overlapped_ex_p->context_p;
		on_recv_packet(net_packet_p, result, num_bytes_transferred);
	}
	else
	{
		TFG_ASSERT(net_overlapped_ex_p->_typeid == Packet::TypeId);
		PacketPtr const net_packet_p = (PacketPtr)net_overlapped_ex_p->object_p;
		on_send_complete(net_packet_p, result, num_bytes_transferred);
	}
	TFG_FUNC_EXIT("");
}

TFG_Result TCPConnection::SendPacket(PacketPtr const in_net_packet)
{
	TFG_FUNC_ENTER();
	TFG_Result _rv = E_FAIL;

	OverlappedExPtr const net_overlapped_ex_p = in_net_packet->GetOverlappedExPtr();
	net_overlapped_ex_p->object_p = in_net_packet;
	net_overlapped_ex_p->_typeid = Packet::TypeId;
	net_overlapped_ex_p->context_p = this;

	in_net_packet->add_ref_object();
	if (ptp_io)
		call_StartThreadpoolIo(ptp_io);
	InterlockedIncrement((LPLONG) & num_pending_overlapped);
	uint32_t wsabuf_array_size = 0;
	WSABUF const *const wsabuf_array = in_net_packet->GetWSABufArray(&wsabuf_array_size);
	int32_t const rvWSASend = call_WSASend(socket, (LPWSABUF)wsabuf_array, wsabuf_array_size, NULL, 0, (LPOVERLAPPED)net_overlapped_ex_p, NULL);
	if ((rvWSASend != 0) && (!((rvWSASend == SOCKET_ERROR) && (WSAGetLastError() == WSA_IO_PENDING))))
	{
		if (ptp_io)
			call_CancelThreadpoolIo(ptp_io);
		InterlockedDecrement((LPLONG) & num_pending_overlapped);
		TFG_ERROR("WSASend failed with some error other than WSA_IO_PENDING");
		in_net_packet->release_object();
		TFG_GOTO_FINALLY(TFG_Level_Error);
	}

	_rv = S_OK;
finally:
	TFG_FUNC_EXIT("");
	return _rv;
}

TCPConnectionPtr TCPConnection::Create(SOCKET const in_socket, PTP_IO const in_ptp_io, CallbackI * const in_net_tcpconnection_callback_i, uint32_t const in_receive_buffer_size, void *const in_context_pv)
{
	TFG_FUNC_ENTER();
	TCPConnectionPtr _rv = 0;

	TCPConnectionPtr const net_tcpconnection_p = new TCPConnection;
	TFG_CHECK(net_tcpconnection_p);

	net_tcpconnection_p->socket = in_socket;
	net_tcpconnection_p->m_CallbackIPtr = in_net_tcpconnection_callback_i;
	net_tcpconnection_p->receive_buffer_size = in_receive_buffer_size;
	net_tcpconnection_p->context_pv = in_context_pv;

	if (Singleton::Get().iocp_h == 0)
	{
		if (in_ptp_io == 0)
		{
			TFG_CHECK((net_tcpconnection_p->ptp_io = call_CreateThreadpoolIo((HANDLE)net_tcpconnection_p->socket, io_completion_callback, net_tcpconnection_p, Singleton::Get().ptp_callback_environ)) != 0);
		}
		else
		{
			net_tcpconnection_p->ptp_io = in_ptp_io;
		}
	}

	TFG_CHECK(SUCCEEDED(net_tcpconnection_p->CallWSARecv()));

	Singleton::Get().m_ObjectTrackerTCPConnection.Add(net_tcpconnection_p);

	_rv = net_tcpconnection_p;
finally:
	if ((_rv == 0) && (net_tcpconnection_p))
		net_tcpconnection_p->Destroy();
	TFG_FUNC_EXIT("");
	return _rv;
}

void TCPConnection::Destroy()
{
	TFG_FUNC_ENTER();

	Singleton::Get().m_ObjectTrackerTCPConnection.Remove(this);

	if (socket)
		TFG_CHECK_NO_GOTO(0 == call_closesocket(socket));

	if (ptp_io)
		call_WaitForThreadpoolIoCallbacks(ptp_io, FALSE);
	if (ptp_io)
		call_CloseThreadpoolIo(ptp_io);

	while (num_pending_overlapped > 0)
	{
		Sleep(0);
	}

	delete this;

	TFG_FUNC_EXIT("");
}

TCPConnection::TCPConnection() : num_pending_overlapped(0), receive_buffer_size(0), m_CallbackIPtr(0), socket(0), ptp_io(0), context_pv(0) {

}

TCPConnection::~TCPConnection() {

}

} // namespace NetNT
} // namespace TFG
