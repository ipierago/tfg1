#include "NetNT.h"
#include "Globals.h"

namespace TFG
{
namespace NetNT
{

NetNT::NetNT(PTP_CALLBACK_ENVIRON const in_ptp_callback_environ, uint32_t const in_packet_buffer_size)
{
	net_globals_init(&g_net_globals, in_ptp_callback_environ, in_packet_buffer_size);
}

NetNT::~NetNT()
{
	net_globals_deinit(&g_net_globals);
}

uint32_t CopyBufferToWSABufArray(void const *const in_src_p, uint32_t const in_src_size, WSABUF const *const in_dest_wsabuf_array, uint32_t const in_dest_wsabuf_array_size, uint32_t const in_dest_offset)
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
