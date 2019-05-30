#include "NetNT.h"
#include "TCPConnector.h"
#include "ObjectTracker.h"
#include "TCPConnection.h"
#include "Singleton.h"
#include "OverlappedEx.h"
#include "Packet.h"

TFG_FILE_SETUP()

namespace TFG
{

namespace NetNT
{

#pragma warning(push)
#pragma warning(disable : 4307)
TFG_TypeId TCPConnector::TypeId = TFG_DJB2_19('e', 'z', 'n', 't', '_', 't', 'c', 'p', 'c', 'o', 'n', 'n', 'e', 'c', 't', 'o', 'r', '_', 't');
#pragma warning(pop)

void TCPConnector::on_connection(TFG_Result const in_result)
{
	TFG_FUNC_ENTER();

	TCPConnectionPtr net_tcpconnection_p = 0;
	TFG_Result hr = in_result;
	if (FAILED(hr))
	{
		TFG_ERROR("Connection failed: %d: %s", hr, TFG_GetErrorString(hr));
	}
	else
	{
		TCPConnection::CallbackI * net_tcpconnection_callback_i = 0;
		uint32_t receive_buffer_size = 0;
		void *context_pv = 0;
		net_tcpconnector_callback_i->OnConnectionAttempted(this, net_tcpconnection_callback_i, &receive_buffer_size, &context_pv);

		net_tcpconnection_p = TCPConnection::Create(socket, ptp_io, net_tcpconnection_callback_i, receive_buffer_size, context_pv);
		ptp_io = 0;
		socket = 0;
		hr = (net_tcpconnection_p ? S_OK : E_FAIL);
		if (SUCCEEDED(hr))
		{
			net_tcpconnector_callback_i->OnConnectionSucceeded(this, net_tcpconnection_p);
		}
	}
	if (FAILED(hr))
	{
		net_tcpconnector_callback_i->OnError(this, hr);
	}

	InterlockedDecrement((LPLONG)&num_pending_overlapped);

	TFG_FUNC_EXIT("");
}

VOID CALLBACK TCPConnector::io_completion_callback(PTP_CALLBACK_INSTANCE in_ptp_callback_instance, PVOID in_context_pv, PVOID in_overlapped_pv, ULONG in_io_result, ULONG_PTR in_num_byte_transfered_p, PTP_IO in_ptp_io)
{
	TFG_FUNC_ENTER();
	OverlappedExPtr const net_overlapped_ex_p = (OverlappedExPtr)in_overlapped_pv;
	if (net_overlapped_ex_p->_typeid == TCPConnection::TypeId)
	{
		TCPConnection::io_completion_callback(in_ptp_callback_instance, in_context_pv, in_overlapped_pv, in_io_result, in_num_byte_transfered_p, in_ptp_io);
	}
	else if (net_overlapped_ex_p->_typeid == Packet::TypeId)
	{
		TCPConnection::io_completion_callback(in_ptp_callback_instance, in_context_pv, in_overlapped_pv, in_io_result, in_num_byte_transfered_p, in_ptp_io);
	}
	else
	{
		TFG_ASSERT(net_overlapped_ex_p->_typeid == TCPConnector::TypeId);
		TCPConnectorPtr const tcpConnectorPtr = (TCPConnectorPtr)net_overlapped_ex_p->object_p;
		TFG_ASSERT(net_overlapped_ex_p == &tcpConnectorPtr->net_overlapped_ex);
		TFG_ASSERT(in_ptp_io == tcpConnectorPtr->ptp_io);
		TFG_Result const hr = HRESULT_FROM_WIN32(in_io_result);
		tcpConnectorPtr->on_connection(hr);
	}
	TFG_FUNC_EXIT("");
}

void TCPConnector::on_queued_completion_status(BOOL const in_rv_GetnQueuedCompletionStatus, uint32_t const in_num_bytes_transferred, uint32_t const in_completion_key, LPOVERLAPPED const in_overlapped_p)
{
	TFG_FUNC_ENTER();
	OverlappedExPtr const net_overlapped_ex_p = (OverlappedExPtr)in_overlapped_p;
	if (net_overlapped_ex_p->_typeid == TCPConnection::TypeId)
	{
		TCPConnectionPtr const net_tcpconnection_p = (TCPConnectionPtr)net_overlapped_ex_p->object_p;
		net_tcpconnection_p->on_queued_completion_status(in_rv_GetnQueuedCompletionStatus, in_num_bytes_transferred, in_completion_key, in_overlapped_p);
	}
	else if (net_overlapped_ex_p->_typeid == Packet::TypeId)
	{
		TCPConnectionPtr const net_tcpconnection_p = (TCPConnectionPtr)net_overlapped_ex_p->context_p;
		net_tcpconnection_p->on_queued_completion_status(in_rv_GetnQueuedCompletionStatus, in_num_bytes_transferred, in_completion_key, in_overlapped_p);
	}
	else
	{
		DWORD num_bytes_transferred = 0;
		DWORD flags = 0;
		BOOL const rv = call_WSAGetOverlappedResult(socket, in_overlapped_p, &num_bytes_transferred, FALSE, &flags);
		TFG_Result const result = (rv ? S_OK : HRESULT_FROM_WIN32(WSAGetLastError()));
		on_connection(result);
	}
	TFG_FUNC_EXIT("");
}

TCPConnectorPtr TCPConnector::Create(const char *const in_pszAddressLocal, int32_t const in_PortLocal, const char *const in_pszAddressRemote, int32_t const in_PortRemote, TCPConnector::CallbackI * const in_net_tcpconnector_callback_i)
{
	TFG_FUNC_ENTER();
	TCPConnectorPtr _rv = 0;

	TCPConnectorPtr const net_tcpconnector_p = new TCPConnector;
	TFG_CHECK(net_tcpconnector_p);

	net_tcpconnector_p->net_tcpconnector_callback_i = in_net_tcpconnector_callback_i;
	net_tcpconnector_p->net_overlapped_ex._typeid = TCPConnector::TypeId;
	net_tcpconnector_p->net_overlapped_ex.object_p = net_tcpconnector_p;
	TFG_CHECK((net_tcpconnector_p->net_overlapped_ex.overlapped.hEvent = call_CreateEventA(NULL, FALSE, FALSE, NULL)) != 0);

	net_tcpconnector_p->socket = call_WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	TFG_CHECK(net_tcpconnector_p->socket != SOCKET_ERROR);

	if (Singleton::Get().iocp_h)
	{
		TFG_CHECK(call_CreateIoCompletionPort((HANDLE)net_tcpconnector_p->socket, Singleton::Get().iocp_h, TCPConnector::TypeId, 0) == Singleton::Get().iocp_h);
	}
	else
	{
		TFG_CHECK((net_tcpconnector_p->ptp_io = call_CreateThreadpoolIo((HANDLE)net_tcpconnector_p->socket, io_completion_callback, net_tcpconnector_p, Singleton::Get().ptp_callback_environ)) != 0);
	}

	LPFN_CONNECTEX _pfn_ConnectEx = 0;
	GUID _guid;
	_guid = WSAID_CONNECTEX;
	DWORD _dw = 0;
	TFG_CHECK(call_WSAIoctl(net_tcpconnector_p->socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &_guid, sizeof(GUID), &_pfn_ConnectEx, sizeof(LPFN_CONNECTEX), &_dw, NULL, NULL) != SOCKET_ERROR);

	struct sockaddr_in _sockaddr_in_local;
	_sockaddr_in_local.sin_family = AF_INET;
	_sockaddr_in_local.sin_addr.s_addr = inet_addr(in_pszAddressLocal);
	_sockaddr_in_local.sin_port = htons((u_short)in_PortLocal);
	TFG_CHECK(call_bind(net_tcpconnector_p->socket, (SOCKADDR *)&_sockaddr_in_local, sizeof(_sockaddr_in_local)) != SOCKET_ERROR);

	struct sockaddr_in _sockaddr_in_remote;
	_sockaddr_in_remote.sin_family = AF_INET;
	_sockaddr_in_remote.sin_addr.s_addr = inet_addr(in_pszAddressRemote);
	_sockaddr_in_remote.sin_port = htons((u_short)in_PortRemote);

	if (net_tcpconnector_p->ptp_io)
		call_StartThreadpoolIo(net_tcpconnector_p->ptp_io);
	InterlockedIncrement((LPLONG)&net_tcpconnector_p->num_pending_overlapped);
	BOOL const _rvConnectEx = _pfn_ConnectEx(net_tcpconnector_p->socket, (struct sockaddr *)&_sockaddr_in_remote, sizeof(_sockaddr_in_remote), NULL, 0, NULL, (LPOVERLAPPED)&net_tcpconnector_p->net_overlapped_ex);
	if (_rvConnectEx == FALSE)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			TFG_ERROR("ConnectEx failed for some other reason than ERROR_IO_PENDING");
			if (net_tcpconnector_p->ptp_io)
				call_CancelThreadpoolIo(net_tcpconnector_p->ptp_io);
			InterlockedDecrement((LPLONG)&net_tcpconnector_p->num_pending_overlapped);
			TFG_GOTO_FINALLY(TFG_Level_Error);
		}
	}

	Singleton::Get().m_ObjectTrackerTCPConnector.Add(net_tcpconnector_p);

	_rv = net_tcpconnector_p;
finally:
	if ((_rv == 0) && net_tcpconnector_p)
	{
		net_tcpconnector_p->Destroy();
	}
	TFG_FUNC_EXIT("");
	return _rv;
}

void TCPConnector::Destroy()
{
	TFG_FUNC_ENTER();

	Singleton::Get().m_ObjectTrackerTCPConnector.Remove(this);

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

	call_CloseHandle(net_overlapped_ex.overlapped.hEvent);

	delete this;

	TFG_FUNC_EXIT("");
}

} // namespace NetNT
} // namespace TFG
