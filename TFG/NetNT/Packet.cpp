#include "NetNT.h"
#include "Packet.h"
#include "Globals.h"
#include "ThreadSingleton.h"

TFG_FILE_SETUP()

#define TFG_NETNT_THREADSINGLETON_FREE_LIST_HIGH_WATER 256
#define TFG_NETNT_THREADSINGLETON_FREE_BUFFERS_HIGH_WATER 256

namespace TFG
{

namespace NetNT
{

#pragma warning(push)
#pragma warning(disable : 4307)
TFG_TypeId Packet::TypeId = TFG_DJB2_16('e', 'z', 'n', 't', '_', 'u', 'd', 'p', 'p', 'a', 'c', 'k', 'e', 't', '_', 't');
#pragma warning(pop)

Packet::Packet()
{
	TFG_FUNC_ENTER();
	net_overlapped_ex.overlapped.hEvent = call_CreateEventA(NULL, FALSE, FALSE, NULL);
	TFG_ASSERT(net_overlapped_ex.overlapped.hEvent);
	TFG_FUNC_EXIT("");
}

Packet::~Packet()
{
	if (net_overlapped_ex.overlapped.hEvent)
	{
		call_CloseHandle(net_overlapped_ex.overlapped.hEvent);
		net_overlapped_ex.overlapped.hEvent = 0;
	}
	std::for_each(wsabuf_vec.begin(), wsabuf_vec.end(), [](WSABUF &wsabuf) { delete[] wsabuf.buf; });
}

// Does not preserve contents
TFG_Result Packet::Resize(int32_t const size)
{
	TFG_FUNC_ENTER();
	TFG_Result rv = E_FAIL;

	uint32_t buffer_index = 0;
	uint32_t remaining_bytes = size;
	while (remaining_bytes > 0)
	{
		if (buffer_index >= wsabuf_vec.size())
		{
			ThreadSingleton &threadSingleton = ThreadSingleton::Instance();
			void *buffer_p(0);
			if (!threadSingleton.free_buffers_std_ptrvec.empty())
			{
				buffer_p = threadSingleton.free_buffers_std_ptrvec.back();
				threadSingleton.free_buffers_std_ptrvec.pop_back();
			}
			else
			{
				buffer_p = new CHAR[g_net_globals.packet_buffer_size];
			}
			TFG_ASSERT(buffer_p);
			WSABUF wsabuf;
			wsabuf.buf = (CHAR *)buffer_p;
			wsabuf.len = std::min(remaining_bytes, g_net_globals.packet_buffer_size);
			wsabuf_vec.push_back(wsabuf);
			remaining_bytes -= wsabuf.len;
		}
		else
		{
			WSABUF &wsabuf = wsabuf_vec.at(buffer_index);
			wsabuf.len = std::min(remaining_bytes, g_net_globals.packet_buffer_size);
			remaining_bytes -= wsabuf.len;
		}
		++buffer_index;
	}
	for (uint32_t i = buffer_index; i < wsabuf_vec.size(); ++i)
	{
		ThreadSingleton &threadSingleton = ThreadSingleton::Instance();
		WSABUF &wsabuf = wsabuf_vec.at(i);
		if (threadSingleton.free_buffers_std_ptrvec.size() < TFG_NETNT_THREADSINGLETON_FREE_BUFFERS_HIGH_WATER)
		{
			threadSingleton.free_buffers_std_ptrvec.push_back(wsabuf.buf);
		}
		else
		{
			delete[] wsabuf.buf;
			wsabuf.buf = 0;
		}
	}
	wsabuf_vec.erase(wsabuf_vec.begin() + buffer_index, wsabuf_vec.end());

	rv = S_OK;
	//finally:
	TFG_FUNC_EXIT("");
	return rv;
}

Packet *Packet::Create()
{
	TFG_FUNC_ENTER();
	Packet *rv = 0;

	Packet *net_packet_p = 0;

	ThreadSingleton &threadSingleton = ThreadSingleton::Instance();
	if (!threadSingleton.free_packets_std_ptrvec.empty())
	{
		net_packet_p = threadSingleton.free_packets_std_ptrvec.back();
		threadSingleton.free_packets_std_ptrvec.pop_back();
	}
	else
	{
		net_packet_p = new Packet();
	}

	net_packet_p->interface_ref_count = 1;
	net_packet_p->object_ref_count = 1;

	InterlockedIncrement64(&g_net_globals.num_packets_in_use);

	rv = net_packet_p;

	//finally:
	if ((rv == 0 && net_packet_p))
		net_packet_p->Release();
	TFG_FUNC_EXIT("return 0x%0*" PRIxPTR, PRIxPTR_WIDTH, rv);
	return rv;
}

namespace
{
uint32_t net_compute_size(WSABUF const *const in_wsabuf_array, uint32_t const in_array_size)
{
	uint32_t rv = 0;
	for (uint32_t i = 0; i < in_array_size; ++i)
	{
		WSABUF const *const wsabuf_p = &in_wsabuf_array[i];
		rv += wsabuf_p->len;
	}
	return rv;
}
} // namespace

uint32_t Packet::ComputeSize() const
{
	uint32_t array_size = 0;
	WSABUF const *const wsabuf_array = GetWSABufArray(&array_size);
	return net_compute_size(wsabuf_array, array_size);
}

uint32_t Packet::AppendData(void const *const in_src_p, uint32_t const in_size)
{
	TFG_FUNC_ENTER();
	uint32_t num_bytes_copied = 0;

	uint32_t const old_size = ComputeSize();
	uint32_t const new_size = old_size + in_size;
	TFG_CHECK(SUCCEEDED(Resize(new_size)));

	uint32_t wsabuf_array_size = 0;
	WSABUF const *const wsabuf_array = GetWSABufArray(&wsabuf_array_size);
	num_bytes_copied = CopyBufferToWSABufArray(in_src_p, in_size, wsabuf_array, wsabuf_array_size, old_size);

finally:
	TFG_FUNC_EXIT("");
	return num_bytes_copied;
}

uint32_t Packet::GetData(void *const in_dest_p, uint32_t const in_capacity)
{
	TFG_FUNC_ENTER();
	uint32_t num_bytes_copied = 0;

	uint32_t const wsabuf_count = wsabuf_vec.size();
	uint32_t wsabuf_index = 0;
	while ((wsabuf_index < wsabuf_count) && (num_bytes_copied < in_capacity))
	{
		WSABUF const &wsabuf = wsabuf_vec.at(wsabuf_index);
		uint32_t const remaining_capacity = in_capacity - num_bytes_copied;
		uint32_t const bytes_to_copy_this_step = std::min((uint32_t)wsabuf.len, remaining_capacity);
		void *const dest_pv = (uint8_t *)in_dest_p + num_bytes_copied;
		memcpy(dest_pv, wsabuf.buf, bytes_to_copy_this_step);
		num_bytes_copied += bytes_to_copy_this_step;
		++wsabuf_index;
	}

	//finally:
	TFG_FUNC_EXIT("");
	return num_bytes_copied;
}

void Packet::on_zero_interface_ref_count()
{
	TFG_FUNC_ENTER();

	TFG_FUNC_EXIT("");
}

void Packet::on_zero_object_ref_count()
{
	TFG_FUNC_ENTER();
	ThreadSingleton &threadSingleton = ThreadSingleton::Instance();

	Resize(0);
	if (threadSingleton.free_packets_std_ptrvec.size() < TFG_NETNT_THREADSINGLETON_FREE_LIST_HIGH_WATER)
	{
		threadSingleton.free_packets_std_ptrvec.push_back(this);
	}
	else
	{
		delete this;
	}

	InterlockedDecrement64(&g_net_globals.num_packets_in_use);

	TFG_FUNC_EXIT("");
}

int32_t Packet::AddRef()
{
	int32_t rv = -1;
	add_ref_object();
	rv = InterlockedIncrement((LPLONG)&interface_ref_count);
	return rv;
}

int32_t Packet::Release()
{
	int32_t rv = -1;
	int32_t const new_ref_count = InterlockedDecrement((LPLONG)&interface_ref_count);
	if (new_ref_count == 0)
	{
		on_zero_interface_ref_count();
	}
	release_object();
	rv = new_ref_count;
	return rv;
}

int32_t Packet::add_ref_object()
{
	int32_t const rv = InterlockedIncrement((LPLONG)&object_ref_count);
	return rv;
}

int32_t Packet::release_object()
{
	int32_t rv = -1;
	int32_t const new_ref_count = InterlockedDecrement((LPLONG)&object_ref_count);
	if (new_ref_count == 0)
	{
		on_zero_object_ref_count();
	}
	rv = new_ref_count;
	return rv;
}

} // namespace NetNT

} // namespace TFG
