#include "NetNT.h"
#include "Globals.h"
#include "OverlappedEx.h"
#include "UDPSocket.h"
#include "Packet.h"

TFG_FILE_SETUP()

namespace TFG
{

namespace NetNT
{

Globals g_net_globals;
int32_t g_net_init_count = 0;

typedef struct net_t
{
	uint32_t num_threads;
	HANDLE *thread_handles_p;
} net_t;
static net_t g_net;

DWORD WINAPI net_iocp_completion_port_thread_main(LPVOID in_pv)
{
	TFG_FUNC_ENTER();
	UNREFERENCED_PARAMETER(in_pv);
	DWORD rv = 1;
	for (;;)
	{
		DWORD num_bytes_transferred = 0;
		ULONG_PTR completion_key = 0;
		LPOVERLAPPED overlapped_p = 0;
		BOOL const rv_GetQueuedCompletionStatus = call_GetQueuedCompletionStatus(g_net_globals.iocp_h, &num_bytes_transferred, &completion_key, &overlapped_p, INFINITE);
		if (completion_key == 0)
		{
			break;
		}

		OverlappedExPtr const net_overlapped_ex_p = (OverlappedExPtr)overlapped_p;

		if (completion_key == UDPSocket::TypeId)
		{
			if (net_overlapped_ex_p->_typeid == UDPSocket::TypeId)
			{
				UDPSocketPtr const net_udpsocket_p = (UDPSocketPtr)net_overlapped_ex_p->object_p;
				(*net_udpsocket_p).OnQueuedCompletionStatus(rv_GetQueuedCompletionStatus, num_bytes_transferred, completion_key, overlapped_p);
			}
			else if (net_overlapped_ex_p->_typeid == Packet::TypeId)
			{
				UDPSocketPtr const net_udpsocket_p = (UDPSocketPtr)net_overlapped_ex_p->context_p;
				(*net_udpsocket_p).OnQueuedCompletionStatus(rv_GetQueuedCompletionStatus, num_bytes_transferred, completion_key, overlapped_p);
			}
		}
#if 0
		else if (net_overlapped_ex_p->_typeid == TCPConnection::TypeId)
		{
			TCPConnectionPtr const net_tcpconnection_p = (TCPConnectionPtr)net_overlapped_ex_p->object_p;
			net_tcpconnection_on_queued_completion_status(net_tcpconnection_p, rv_GetQueuedCompletionStatus, num_bytes_transferred, completion_key, overlapped_p);
		}
#endif
#if 0
		else if (net_overlapped_ex_p->_typeid == Packet::TypeId)
		{
			TCPConnectionPtr const net_tcpconnection_p = (TCPConnectionPtr)net_overlapped_ex_p->context_p;
			net_tcpconnection_on_queued_completion_status(net_tcpconnection_p, rv_GetQueuedCompletionStatus, num_bytes_transferred, completion_key, overlapped_p);
		}
#endif
#if 0		
		else if (net_overlapped_ex_p->_typeid == TCPAcceptor::TypeId)
		{
			TCPAcceptorPtr const net_tcpacceptor_p = (TCPAcceptorPtr)net_overlapped_ex_p->object_p;
			net_tcpacceptor_on_queued_completion_status(net_tcpacceptor_p, rv_GetQueuedCompletionStatus, num_bytes_transferred, completion_key, overlapped_p);
		}
#endif
#if 0
		else if (net_overlapped_ex_p->_typeid == TCPConnector::TypeId)
		{
			TCPConnectorPtr const net_tcpconnector_p = (TCPConnectorPtr)net_overlapped_ex_p->object_p;
			net_tcpconnector_on_queued_completion_status(net_tcpconnector_p, rv_GetQueuedCompletionStatus, num_bytes_transferred, completion_key, overlapped_p);
		}
#endif
		else
		{
			TFG_CHECK(FALSE);
		}
	}

	rv = 0;
finally:
	TFG_FUNC_EXIT("");
	return rv;
}

TFG_Result NetNT_Init(PTP_CALLBACK_ENVIRON const in_ptp_callback_environ, uint32_t const in_num_threads, uint32_t const in_packet_buffer_size)
{
	TFG_FUNC_ENTER();
	TFG_Result rv = E_FAIL;

	memset(&g_net, 0, sizeof(g_net));

	TFG_CHECK(InterlockedIncrement((LPLONG)&g_net_init_count) == 1);

	TFG_CHECK(SUCCEEDED(net_globals_init(&g_net_globals, in_ptp_callback_environ, in_packet_buffer_size)));

	WORD const version_requested = MAKEWORD(2, 2);
	WSADATA wsa_data;
	TFG_CHECK(call_WSAStartup(version_requested, &wsa_data) == 0);
	int32_t const major_version = HIBYTE(wsa_data.wVersion);
	int32_t const minor_version = LOBYTE(wsa_data.wVersion);
	TFG_CHECK((major_version == 2) && (minor_version == 2));

	// TODO: Only do this when tp is null?
	if (in_num_threads)
	{
		TFG_CHECK((g_net_globals.iocp_h = call_CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, in_num_threads)) != 0);
		g_net.num_threads = in_num_threads;
		g_net.thread_handles_p = new HANDLE[in_num_threads];
		for (uint32_t i = 0; i < in_num_threads; ++i)
		{
			DWORD ignored = 0;
			TFG_CHECK((g_net.thread_handles_p[i] = call_CreateThread(NULL, 0, net_iocp_completion_port_thread_main, NULL, 0, &ignored)) != 0);
		}
	}

	TFG_DEBUG("net successfully initialized");
	rv = S_OK;
finally:
	if (FAILED(rv))
		NetNT_Deinit();
	TFG_FUNC_EXIT("");
	return rv;
}

void NetNT_Deinit()
{
	TFG_FUNC_ENTER();

	// TODO: Track and destroy sockets, connections, connector and acceptors

	TFG_CHECK_NO_GOTO(call_WSACleanup() != SOCKET_ERROR);

	for (uint32_t i = 0; i < g_net.num_threads; ++i)
	{
		TFG_CHECK_NO_GOTO(call_PostQueuedCompletionStatus(g_net_globals.iocp_h, 0, 0, 0));
	}
	for (uint32_t i = 0; i < g_net.num_threads; ++i)
	{
		TFG_CHECK_NO_GOTO(call_WaitForSingleObject(g_net.thread_handles_p[i], INFINITE) != WAIT_FAILED);
	}
	if (g_net.thread_handles_p)
	{
		delete[] g_net.thread_handles_p;
		g_net.thread_handles_p = 0;
	}
	if (g_net_globals.iocp_h)
		TFG_CHECK_NO_GOTO(call_CloseHandle(g_net_globals.iocp_h));

	net_globals_deinit(&g_net_globals);

	TFG_DEBUG("net successfully shutdown");

	TFG_FUNC_EXIT("");
}

TFG_Result NetNT_InitSockAddrIn(struct sockaddr_in *const in_sockaddr_in_p, const char *const in_address_p, int32_t const in_port)
{
	in_sockaddr_in_p->sin_family = AF_INET;
	in_sockaddr_in_p->sin_port = htons((u_short)in_port);
	in_sockaddr_in_p->sin_addr.s_addr = inet_addr(in_address_p);
	return S_OK;
}

uint32_t NetNT_GetPacketBufferSize()
{
	return g_net_globals.packet_buffer_size;
}

uint32_t NetNT_CopyWSABufArrayToWSABufArray(WSABUF const *const in_src_wsabuf_array, uint32_t const in_src_wsabuf_array_size, uint32_t const in_src_offset, WSABUF const *const in_dest_wsabuf_array, uint32_t const in_dest_wsabuf_array_size, uint32_t const in_dest_offset, uint32_t const in_count)
{
	TFG_FUNC_ENTER();
	uint32_t num_bytes_copied = 0;

	uint32_t src_wsabuf_index = 0;
	uint32_t src_index_this_buffer = 0;
	uint32_t src_previous_capacity = 0;
	while (src_wsabuf_index < in_src_wsabuf_array_size)
	{
		uint32_t capacity_this_buffer = in_src_wsabuf_array[src_wsabuf_index].len;
		if (src_previous_capacity + capacity_this_buffer > in_src_offset)
		{
			src_index_this_buffer = in_src_offset - src_previous_capacity;
			break;
		}
		src_previous_capacity += capacity_this_buffer;
		++src_wsabuf_index;
	}

	uint32_t dest_wsabuf_index = 0;
	uint32_t dest_index_this_buffer = 0;
	uint32_t dest_previous_capacity = 0;
	while (dest_wsabuf_index < in_dest_wsabuf_array_size)
	{
		uint32_t capacity_this_buffer = in_dest_wsabuf_array[dest_wsabuf_index].len;
		if (dest_previous_capacity + capacity_this_buffer > in_dest_offset)
		{
			dest_index_this_buffer = in_dest_offset - dest_previous_capacity;
			break;
		}
		dest_previous_capacity += capacity_this_buffer;
		++dest_wsabuf_index;
	}

	while ((src_wsabuf_index < in_src_wsabuf_array_size) && (dest_wsabuf_index < in_dest_wsabuf_array_size) && (num_bytes_copied < in_count))
	{
		WSABUF const *const src_wsabuf_p = &in_src_wsabuf_array[src_wsabuf_index];
		WSABUF const *const dest_wsabuf_p = &in_dest_wsabuf_array[dest_wsabuf_index];
		uint32_t const src_remain_bytes_this_buffer = src_wsabuf_p->len - src_index_this_buffer;
		uint32_t const dest_remain_bytes_this_buffer = dest_wsabuf_p->len - dest_index_this_buffer;
		uint32_t const remain_bytes_to_copy = in_count - num_bytes_copied;
		uint32_t const bytes_to_copy_this_step = std::min(src_remain_bytes_this_buffer, std::min(dest_remain_bytes_this_buffer, remain_bytes_to_copy));
		void *const dest_pv = (uint8_t *)(dest_wsabuf_p->buf) + dest_index_this_buffer;
		void *const src_pv = (uint8_t *)(src_wsabuf_p->buf) + src_index_this_buffer;
		memcpy(dest_pv, src_pv, bytes_to_copy_this_step);
		num_bytes_copied += bytes_to_copy_this_step;
		src_index_this_buffer += bytes_to_copy_this_step;
		TFG_ASSERT(src_index_this_buffer <= src_wsabuf_p->len);
		if (src_index_this_buffer == src_wsabuf_p->len)
		{
			src_index_this_buffer = 0;
			++src_wsabuf_index;
		}
		dest_index_this_buffer += bytes_to_copy_this_step;
		TFG_ASSERT(dest_index_this_buffer <= dest_wsabuf_p->len);
		if (dest_index_this_buffer >= dest_wsabuf_p->len)
		{
			dest_index_this_buffer = 0;
			++dest_wsabuf_index;
		}
	}

	TFG_FUNC_EXIT("");
	return num_bytes_copied;
}

uint32_t NetNT_CopyBufferToWSABufArray(void const *const in_src_p, uint32_t const in_src_size, WSABUF const *const in_dest_wsabuf_array, uint32_t const in_dest_wsabuf_array_size, uint32_t const in_dest_offset)
{
	uint32_t num_bytes_copied = 0;

	uint32_t dest_wsabuf_index = 0;
	uint32_t dest_index_this_buffer = 0;
	uint32_t dest_previous_capacity = 0;
	while (dest_wsabuf_index < in_dest_wsabuf_array_size)
	{
		uint32_t capacity_this_buffer = in_dest_wsabuf_array[dest_wsabuf_index].len;
		if (dest_previous_capacity + capacity_this_buffer > in_dest_offset)
		{
			dest_index_this_buffer = in_dest_offset - dest_previous_capacity;
			break;
		}
		dest_previous_capacity += capacity_this_buffer;
		++dest_wsabuf_index;
	}

	uint32_t src_index = 0;
	while ((src_index < in_src_size) && (dest_wsabuf_index < in_dest_wsabuf_array_size))
	{
		WSABUF const *const dest_wsabuf_p = &in_dest_wsabuf_array[dest_wsabuf_index];
		uint32_t const dest_remain_bytes_this_buffer = dest_wsabuf_p->len - dest_index_this_buffer;
		uint32_t const remain_bytes_to_copy = in_src_size - num_bytes_copied;
		uint32_t const bytes_to_copy_this_step = std::min(dest_remain_bytes_this_buffer, remain_bytes_to_copy);
		void *const dest_pv = (uint8_t *)(dest_wsabuf_p->buf) + dest_index_this_buffer;
		void *const src_pv = (uint8_t *)(in_src_p) + src_index;
		memcpy(dest_pv, src_pv, bytes_to_copy_this_step);
		num_bytes_copied += bytes_to_copy_this_step;
		src_index += bytes_to_copy_this_step;
		dest_index_this_buffer += bytes_to_copy_this_step;
		if (dest_index_this_buffer >= dest_wsabuf_p->len)
		{
			dest_index_this_buffer = 0;
			++dest_wsabuf_index;
		}
	}

	return num_bytes_copied;
}

} // namespace NetNT

} // namespace TFG
