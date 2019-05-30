#include "NetNT_pch.h"

#include "TCPConnector.h"

#include "ObjectTracker.h"
#include "TCPConnection.h"
#include "Globals.h"
#include "OverlappedEx.h"
#include "Packet.h"

#include <NetNT/NetNT.h>

#include <Std/Std.h>
#include <Call/call.h>
#include <Std/Allocator.h>
#include <Std/djb2.h>
#include <Std/error.h>

TFG_FILE_SETUP()

typedef struct NetNT_TCPConnector
{
	int32_t num_pending_overlapped;
	SOCKET socket;
	NetNT_TCPConnectorCallbackI net_tcpconnector_callback_i;
	NetNT_OverlappedEx net_overlapped_ex;
	PTP_IO ptp_io;
} NetNT_TCPConnector;

#pragma warning(push)
#pragma warning(disable : 4307)
Std_TypeId net_tcpconnector_typeid = TFG_DJB2_19('e', 'z', 'n', 't', '_', 't', 'c', 'p', 'c', 'o', 'n', 'n', 'e', 'c', 't', 'o', 'r', '_', 't');
#pragma warning(pop)

static void net_tcpconnector_on_connection(NetNT_TCPConnectorPtr const in_this, TFG_Result const in_result)
{
	TFG_FUNC_ENTER();

	NetNT_TCPConnectionPtr net_tcpconnection_p = 0;
	TFG_Result hr = in_result;
	if (FAILED(hr))
	{
		TFG_ERROR("Connection failed: %d: %s", hr, Std_GetErrorString(hr));
	}
	else
	{
		NetNT_TCPConnectionCallbackI net_tcpconnection_callback_i = 0;
		uint32_t receive_buffer_size = 0;
		void *context_pv = 0;
		NetNT_TCPConnectorCallbackI_OnConnectionAttempted(in_this->net_tcpconnector_callback_i, in_this, &net_tcpconnection_callback_i, &receive_buffer_size, &context_pv);

		net_tcpconnection_p = net_tcpconnection_create(in_this->socket, in_this->ptp_io, net_tcpconnection_callback_i, receive_buffer_size, context_pv);
		in_this->ptp_io = 0;
		in_this->socket = 0;
		hr = (net_tcpconnection_p ? S_OK : E_FAIL);
		if (SUCCEEDED(hr))
		{
			NetNT_TCPConnectorCallbackI_OnConnectionSucceeded(in_this->net_tcpconnector_callback_i, in_this, net_tcpconnection_p);
		}
	}
	if (FAILED(hr))
	{
		NetNT_TCPConnectorCallbackI_OnError(in_this->net_tcpconnector_callback_i, in_this, hr);
	}

	InterlockedDecrement((LPLONG)&in_this->num_pending_overlapped);

	TFG_FUNC_EXIT("");
}

VOID CALLBACK net_tcpconnector_io_completion_callback(PTP_CALLBACK_INSTANCE in_ptp_callback_instance, PVOID in_context_pv, PVOID in_overlapped_pv, ULONG in_io_result, ULONG_PTR in_num_byte_transfered_p, PTP_IO in_ptp_io)
{
	TFG_FUNC_ENTER();
	NetNT_OverlappedExPtr const net_overlapped_ex_p = (NetNT_OverlappedExPtr)in_overlapped_pv;
	if (net_overlapped_ex_p->_typeid == net_tcpconnection_typeid)
	{
		net_tcpconnection_io_completion_callback(in_ptp_callback_instance, in_context_pv, in_overlapped_pv, in_io_result, in_num_byte_transfered_p, in_ptp_io);
	}
	else if (net_overlapped_ex_p->_typeid == net_packet_typeid)
	{
		net_tcpconnection_io_completion_callback(in_ptp_callback_instance, in_context_pv, in_overlapped_pv, in_io_result, in_num_byte_transfered_p, in_ptp_io);
	}
	else
	{
		TFG_ASSERT(net_overlapped_ex_p->_typeid == net_tcpconnector_typeid);
		NetNT_TCPConnectorPtr const in_this = (NetNT_TCPConnectorPtr)net_overlapped_ex_p->object_p;
		TFG_ASSERT(net_overlapped_ex_p == &in_this->net_overlapped_ex);
		TFG_ASSERT(in_ptp_io == in_this->ptp_io);
		TFG_Result const hr = HRESULT_FROM_WIN32(in_io_result);
		net_tcpconnector_on_connection(in_this, hr);
	}
	TFG_FUNC_EXIT("");
}

void net_tcpconnector_on_queued_completion_status(NetNT_TCPConnectorPtr const in_this, BOOL const in_rv_GetnQueuedCompletionStatus, uint32_t const in_num_bytes_transferred, uint32_t const in_completion_key, LPOVERLAPPED const in_overlapped_p)
{
	TFG_FUNC_ENTER();
	NetNT_OverlappedExPtr const net_overlapped_ex_p = (NetNT_OverlappedExPtr)in_overlapped_p;
	if (net_overlapped_ex_p->_typeid == net_tcpconnection_typeid)
	{
		NetNT_TCPConnectionPtr const net_tcpconnection_p = (NetNT_TCPConnectionPtr)net_overlapped_ex_p->object_p;
		net_tcpconnection_on_queued_completion_status(net_tcpconnection_p, in_rv_GetnQueuedCompletionStatus, in_num_bytes_transferred, in_completion_key, in_overlapped_p);
	}
	else if (net_overlapped_ex_p->_typeid == net_packet_typeid)
	{

		NetNT_TCPConnectionPtr const net_tcpconnection_p = (NetNT_TCPConnectionPtr)net_overlapped_ex_p->context_p;
		net_tcpconnection_on_queued_completion_status(net_tcpconnection_p, in_rv_GetnQueuedCompletionStatus, in_num_bytes_transferred, in_completion_key, in_overlapped_p);
	}
	else
	{
		DWORD num_bytes_transferred = 0;
		DWORD flags = 0;
		BOOL const rv = call_WSAGetOverlappedResult(in_this->socket, in_overlapped_p, &num_bytes_transferred, FALSE, &flags);
		TFG_Result const result = (rv ? S_OK : HRESULT_FROM_WIN32(WSAGetLastError()));
		net_tcpconnector_on_connection(in_this, result);
	}
	TFG_FUNC_EXIT("");
}

NetNT_TCPConnectorPtr NetNT_TCPConnector_Create(const char *const in_pszAddressLocal, int32_t const in_PortLocal, const char *const in_pszAddressRemote, int32_t const in_PortRemote, NetNT_TCPConnectorCallbackI const in_net_tcpconnector_callback_i)
{
	TFG_FUNC_ENTER();
	NetNT_TCPConnectorPtr _rv = 0;

	NetNT_TCPConnectorPtr const net_tcpconnector_p = (NetNT_TCPConnectorPtr)Std_AllocatorI_Alloc(g_net_globals.std_allocator_i_p, sizeof(NetNT_TCPConnector));
	TFG_CHECK(net_tcpconnector_p);

	call_memset(net_tcpconnector_p, 0, sizeof(*net_tcpconnector_p));

	net_tcpconnector_p->net_tcpconnector_callback_i = in_net_tcpconnector_callback_i;
	net_tcpconnector_p->net_overlapped_ex._typeid = net_tcpconnector_typeid;
	net_tcpconnector_p->net_overlapped_ex.object_p = net_tcpconnector_p;
	TFG_CHECK((net_tcpconnector_p->net_overlapped_ex.overlapped.hEvent = call_CreateEventA(NULL, FALSE, FALSE, NULL)) != 0);

	net_tcpconnector_p->socket = call_WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	TFG_CHECK(net_tcpconnector_p->socket != SOCKET_ERROR);

	if (g_net_globals.iocp_h)
	{
		TFG_CHECK(call_CreateIoCompletionPort((HANDLE)net_tcpconnector_p->socket, g_net_globals.iocp_h, net_tcpconnector_typeid, 0) == g_net_globals.iocp_h);
	}
	else
	{
		TFG_CHECK((net_tcpconnector_p->ptp_io = call_CreateThreadpoolIo((HANDLE)net_tcpconnector_p->socket, net_tcpconnector_io_completion_callback, net_tcpconnector_p, g_net_globals.ptp_callback_environ)) != 0);
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
	BOOL const _rvConnectEx = call_ConnectEx(_pfn_ConnectEx, net_tcpconnector_p->socket, (struct sockaddr *)&_sockaddr_in_remote, sizeof(_sockaddr_in_remote), NULL, 0, NULL, (LPOVERLAPPED)&net_tcpconnector_p->net_overlapped_ex);
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

	TFG_CHECK(SUCCEEDED(net_object_tracker_add(&g_net_globals.tcpconnector_object_tracker, net_tcpconnector_p)));

	_rv = net_tcpconnector_p;
finally:
	if ((_rv == 0) && net_tcpconnector_p)
	{
		NetNT_TCPConnector_Destroy(net_tcpconnector_p);
	}
	TFG_FUNC_EXIT("");
	return _rv;
}

void NetNT_TCPConnector_Destroy(NetNT_TCPConnectorPtr const in_this)
{
	TFG_FUNC_ENTER();

	net_object_tracker_remove(&g_net_globals.tcpconnector_object_tracker, in_this);

	if (in_this->socket)
		TFG_CHECK_NO_GOTO(0 == call_closesocket(in_this->socket));
	if (in_this->ptp_io)
		call_WaitForThreadpoolIoCallbacks(in_this->ptp_io, FALSE);

	if (in_this->ptp_io)
		call_CloseThreadpoolIo(in_this->ptp_io);

	while (in_this->num_pending_overlapped > 0)
	{
		Sleep(0);
	}

	call_CloseHandle(in_this->net_overlapped_ex.overlapped.hEvent);

	Std_AllocatorI_Free(g_net_globals.std_allocator_i_p, in_this);

	TFG_FUNC_EXIT("");
}
