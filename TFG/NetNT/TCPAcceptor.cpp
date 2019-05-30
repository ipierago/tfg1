#include "NetNT.h"
#include "TCPAcceptor.h"
#include "ObjectTracker.h"
#include "OverlappedEx.h"
#include "TCPConnection.h"
#include "Singleton.h"

TFG_FILE_SETUP()

namespace TFG {

namespace NetNT {


#pragma warning(push)
#pragma warning(disable : 4307)
TFG_TypeId TCPAcceptor::TypeId = (TFG_TypeId)TFG_DJB2_18('e', 'z', 'n', 't', '_', 't', 'c', 'p', 'a', 'c', 'c', 'e', 'p', 't', 'o', 'r', '_', 't');
#pragma warning(pop)

TFG_Result TCPAcceptor::accept_ex()
{
	TFG_FUNC_ENTER();
	TFG_Result rv = E_FAIL;

	accept_socket = call_WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	TFG_CHECK(accept_socket != SOCKET_ERROR);

	if (ptp_io)
		call_StartThreadpoolIo(ptp_io);
	InterlockedIncrement((LPLONG)&num_pending_overlapped);
	BOOL const rvAcceptEx = accept_ex_pfn(socket, accept_socket, output_buf, 0, sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16, &dwBytesReceived, (LPOVERLAPPED)&net_overlapped_ex);
	if ((rvAcceptEx != TRUE) && (WSAGetLastError() != WSA_IO_PENDING))
	{
		TFG_ERROR("AcceptEx failed with some error other than WSA_IO_PENDING");
		if (ptp_io)
			call_CancelThreadpoolIo(ptp_io);
		InterlockedDecrement((LPLONG)&num_pending_overlapped);
		TFG_GOTO_FINALLY(TFG_Level_Error);
	}

	rv = S_OK;
finally:
	TFG_FUNC_EXIT("");
	return rv;
}

void TCPAcceptor::on_connection(TFG_Result const in_result)
{
	TFG_FUNC_ENTER();
	TFG_Result hr = in_result;
	if (FAILED(hr))
	{
		TFG_WARNING("tcp accept operation failed: %d: %s", hr, TFG_GetErrorString(hr));
	}
	else
	{
		// TODO: Get struct sockaddr_in from _output_buf and pass it to attempted connection callback
		TCPConnection::CallbackI * net_tcpconnection_callback_i = 0;
		uint32_t receive_buffer_size = 0;
		void *context_pv = 0;
		net_tcpacceptor_callback_i->OnConnectionAttempted(this, &net_tcpconnection_callback_i, &receive_buffer_size, &context_pv);

		if (Singleton::Get().iocp_h)
		{
			TFG_CHECK_NO_GOTO(call_CreateIoCompletionPort((HANDLE)accept_socket, Singleton::Get().iocp_h, TCPConnection::TypeId, 0) == Singleton::Get().iocp_h);
		}

		TCPConnectionPtr const net_tcpconnection_p = TCPConnection::Create(accept_socket, 0, net_tcpconnection_callback_i, receive_buffer_size, context_pv);
		hr = (net_tcpconnection_p ? S_OK : E_FAIL);
		if (SUCCEEDED(hr))
		{
			accept_socket = 0;
			net_tcpacceptor_callback_i->OnConnectionSucceeded(this, net_tcpconnection_p);
		}

		if (accept_socket != 0)
		{
			call_closesocket(accept_socket);
			accept_socket = 0;
		}
		if (SUCCEEDED(hr))
		{
			hr = accept_ex();
		}
	}
	if (FAILED(hr))
	{
		net_tcpacceptor_callback_i->OnError(this, hr);
	}
	InterlockedDecrement((LPLONG)&num_pending_overlapped);
	TFG_FUNC_EXIT("");
}

VOID CALLBACK TCPAcceptor::io_completion_callback(PTP_CALLBACK_INSTANCE in_ptp_callback_instance, PVOID in_context_pv, PVOID in_overlapped_pv, ULONG in_io_result, ULONG_PTR in_num_byte_transfered_p, PTP_IO in_ptp_io)
{
	TFG_FUNC_ENTER();
	UNREFERENCED_PARAMETER(in_num_byte_transfered_p);
	UNREFERENCED_PARAMETER(in_context_pv);
	UNREFERENCED_PARAMETER(in_ptp_callback_instance);
	OverlappedExPtr const net_overlapped_ex_p = (OverlappedExPtr)in_overlapped_pv;
	TFG_ASSERT(net_overlapped_ex_p->_typeid == TCPAcceptor::TypeId);
	TCPAcceptorPtr const tcpAcceptorPtr = (TCPAcceptorPtr)net_overlapped_ex_p->object_p;
	TFG_ASSERT(net_overlapped_ex_p == &tcpAcceptorPtr->net_overlapped_ex);
	TFG_ASSERT(in_ptp_io == tcpAcceptorPtr->ptp_io);
	in_ptp_io;
	TFG_Result hr = HRESULT_FROM_WIN32(in_io_result);
	tcpAcceptorPtr->on_connection(hr);
	TFG_FUNC_EXIT("");
}

void TCPAcceptor::on_queued_completion_status(BOOL const in_rv_GetnQueuedCompletionStatus, uint32_t const in_num_bytes_transferred, uint32_t const in_completion_key, LPOVERLAPPED const in_overlapped_p)
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

	if (net_overlapped_ex_p->_typeid == TCPAcceptor::TypeId)
	{
		on_connection(result);
	}
	else
	{
		TFG_ASSERT(FALSE);
	}
	TFG_FUNC_EXIT("");
}

#define NET_TCPACCEPTOR_LISTEN_BACKLOG 100

TCPAcceptorPtr TCPAcceptor::Create(const char *const in_pszAddress, int32_t const in_Port, TCPAcceptor::CallbackI * const in_net_tcpacceptor_callback_i)
{
	TFG_FUNC_ENTER();
	TCPAcceptorPtr _rv = 0;

	TCPAcceptorPtr const net_tcpacceptor_p = new TCPAcceptor;
	TFG_CHECK(net_tcpacceptor_p);

	net_tcpacceptor_p->net_tcpacceptor_callback_i = in_net_tcpacceptor_callback_i;
	TFG_CHECK(in_net_tcpacceptor_callback_i);

	net_tcpacceptor_p->socket = call_WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	TFG_CHECK(net_tcpacceptor_p->socket != SOCKET_ERROR);

	net_tcpacceptor_p->net_overlapped_ex._typeid = TCPAcceptor::TypeId;
	net_tcpacceptor_p->net_overlapped_ex.object_p = net_tcpacceptor_p;
	TFG_CHECK((net_tcpacceptor_p->net_overlapped_ex.overlapped.hEvent = call_CreateEventA(NULL, FALSE, FALSE, NULL)) != 0);

	if (Singleton::Get().iocp_h)
	{
		TFG_CHECK(call_CreateIoCompletionPort((HANDLE)net_tcpacceptor_p->socket, Singleton::Get().iocp_h, TCPAcceptor::TypeId, 0) == Singleton::Get().iocp_h);
	}
	else
	{
		TFG_CHECK((net_tcpacceptor_p->ptp_io = call_CreateThreadpoolIo((HANDLE)net_tcpacceptor_p->socket, io_completion_callback, net_tcpacceptor_p, Singleton::Get().ptp_callback_environ)) != 0);
	}

	struct sockaddr_in _sockaddr_in;
	_sockaddr_in.sin_family = AF_INET;
	_sockaddr_in.sin_addr.s_addr = inet_addr(in_pszAddress);
	_sockaddr_in.sin_port = htons((u_short)in_Port);
	TFG_CHECK(call_bind(net_tcpacceptor_p->socket, (SOCKADDR *)&_sockaddr_in, sizeof(_sockaddr_in)) != SOCKET_ERROR);

	TFG_CHECK(call_listen(net_tcpacceptor_p->socket, NET_TCPACCEPTOR_LISTEN_BACKLOG) != SOCKET_ERROR);

	GUID GuidAcceptEx;
	GuidAcceptEx = WSAID_ACCEPTEX;
	DWORD dwBytes = 0;
	TFG_CHECK(call_WSAIoctl(net_tcpacceptor_p->socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &GuidAcceptEx, sizeof(GuidAcceptEx), &net_tcpacceptor_p->accept_ex_pfn, sizeof(net_tcpacceptor_p->accept_ex_pfn), &dwBytes, NULL, NULL) != SOCKET_ERROR);

	TFG_CHECK(SUCCEEDED(net_tcpacceptor_p->accept_ex()));

	Singleton::Get().m_ObjectTrackerTCPAcceptor.Add(net_tcpacceptor_p);

	_rv = net_tcpacceptor_p;
finally:
	if ((_rv == 0) && (net_tcpacceptor_p))
		net_tcpacceptor_p->Destroy();
	TFG_FUNC_EXIT("");
	return _rv;
}

void TCPAcceptor::Destroy()
{
	TFG_FUNC_ENTER();

	Singleton::Get().m_ObjectTrackerTCPAcceptor.Remove(this);

	if (accept_socket)
		TFG_CHECK_NO_GOTO(0 == call_closesocket(accept_socket));
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

}

}
