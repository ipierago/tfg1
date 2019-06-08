#include "stdafx.h"
#include "netnt.h"
#include "StreamToDGram.h"

#include "packet.h"
#include "singleton.h"

TFG_FILE_SETUP()

namespace TFG {
	namespace NetNT {

StreamToDGram::StreamToDGram(StreamToDGram::CallbackI *const in_CallbackIPtr) : m_CallbackIPtr(in_CallbackIPtr)
{
}

StreamToDGram::~StreamToDGram()
{
	if (dest_packet_p)
	{
		dest_packet_p->Release();
		dest_packet_p = 0;
	}
}

StreamToDGram *StreamToDGram::Create(StreamToDGram::CallbackI *const in_CallbackIPtr)
{
	TFG_FUNC_ENTER();
	StreamToDGramPtr rv = 0;

	StreamToDGramPtr const net_stream_to_dgram_p = new StreamToDGram(in_CallbackIPtr);

	rv = net_stream_to_dgram_p;
//finally:
	if ((rv == 0) && net_stream_to_dgram_p)
	{
		net_stream_to_dgram_p->Destroy();
	}
	TFG_FUNC_EXIT("");
	return rv;
}

void StreamToDGram::Destroy()
{
	TFG_FUNC_ENTER();

	delete this;

	TFG_FUNC_EXIT("");
}

PacketPtr StreamToDGram::CreateDGramPacket(PacketPtr const in_net_packet_p)
{
	TFG_FUNC_ENTER();
	PacketPtr rv = 0;

	PacketPtr const net_packet_p = Packet::Create();
	TFG_CHECK(net_packet_p);

	uint32_t const payload_size = in_net_packet_p->ComputeSize();
	uint32_t const dgram_size = payload_size + sizeof(net_stream_to_dgram_header_t);

	net_stream_to_dgram_header_t net_stream_to_dgram_header;
	net_stream_to_dgram_header.size = (uint16_t)dgram_size;
	net_stream_to_dgram_header.padding = 0;
	net_packet_p->AppendData(&net_stream_to_dgram_header, sizeof(net_stream_to_dgram_header_t));

	TFG_CHECK(SUCCEEDED(net_packet_p->Resize(dgram_size)));

	uint32_t src_wsabuf_array_size = 0;
	WSABUF const *const src_wsabuf_array = in_net_packet_p->GetWSABufArray(&src_wsabuf_array_size);
	uint32_t dest_wsabuf_array_size = 0;
	WSABUF const *const dest_wsabuf_arary = net_packet_p->GetWSABufArray(&dest_wsabuf_array_size);
	TFG_CHECK(CopyWSABufArrayToWSABufArray(src_wsabuf_array, src_wsabuf_array_size, 0, dest_wsabuf_arary, dest_wsabuf_array_size, sizeof(net_stream_to_dgram_header_t), payload_size) == payload_size);

	rv = net_packet_p;
finally:
	if ((rv == 0) && (net_packet_p))
		net_packet_p->Release();
	TFG_FUNC_EXIT("");
	return rv;
}

TFG_Result StreamToDGram::OnRecvPacket(PacketPtr const in_net_packet_p)
{
	TFG_FUNC_ENTER();
	TFG_Result rv = E_FAIL;

	uint32_t src_wsabuf_array_size = 0;
	WSABUF const *const src_wsabuf_array = in_net_packet_p->GetWSABufArray(&src_wsabuf_array_size);
	uint32_t src_wsabuf_index = 0;
	uint32_t src_index_this_buffer = 0;

	while (src_wsabuf_index < src_wsabuf_array_size)
	{
		while ((header_num_bytes_read < sizeof(net_stream_to_dgram_header_t)) && (src_wsabuf_index < src_wsabuf_array_size))
		{
			WSABUF const *const src_wsabuf_p = &src_wsabuf_array[src_wsabuf_index];
			uint8_t const *const src_p = (uint8_t const *)src_wsabuf_p->buf + src_index_this_buffer;
			uint8_t *const dest_p = (uint8_t *)&header + header_num_bytes_read;
			*dest_p = *src_p;
			++header_num_bytes_read;
			++src_index_this_buffer;
			if (header_num_bytes_read == sizeof(net_stream_to_dgram_header_t))
			{
				TFG_CHECK((dest_packet_p = Packet::Create()) != 0);
				uint32_t const dest_packet_size = ntohs(header.size);
				TFG_ASSERT(dest_packet_size < 2048);
				uint32_t const dest_payload_size = dest_packet_size - sizeof(net_stream_to_dgram_header_t);
				TFG_CHECK(SUCCEEDED(dest_packet_p->Resize(dest_payload_size)));
				dest_wsabuf_index = 0;
				dest_index_this_buffer = 0;
			}
			TFG_ASSERT(src_index_this_buffer <= src_wsabuf_p->len);
			if (src_index_this_buffer == src_wsabuf_p->len)
			{
				src_index_this_buffer = 0;
				++src_wsabuf_index;
			}
		}

		if (dest_packet_p)
		{
			uint32_t dest_wsabuf_array_size = 0;
			WSABUF const *const dest_wsabuf_array = dest_packet_p->GetWSABufArray(&dest_wsabuf_array_size);

			while ((src_wsabuf_index < src_wsabuf_array_size) && (dest_wsabuf_index < dest_wsabuf_array_size))
			{
				WSABUF const *const src_wsabuf_p = &src_wsabuf_array[src_wsabuf_index];
				WSABUF const *const dest_wsabuf_p = &dest_wsabuf_array[dest_wsabuf_index];
				uint32_t const src_remain_bytes_this_buffer = src_wsabuf_p->len - src_index_this_buffer;
				uint32_t const dest_remain_bytes_this_buffer = dest_wsabuf_p->len - dest_index_this_buffer;
				uint32_t const bytes_to_copy_this_step = std::min(src_remain_bytes_this_buffer, dest_remain_bytes_this_buffer);
				void *const dest_pv = (uint8_t *)(dest_wsabuf_p->buf) + dest_index_this_buffer;
				void const *const src_pv = (uint8_t const *)(src_wsabuf_p->buf) + src_index_this_buffer;
				memcpy(dest_pv, src_pv, bytes_to_copy_this_step);
				src_index_this_buffer += bytes_to_copy_this_step;
				TFG_ASSERT(src_index_this_buffer <= src_wsabuf_p->len);
				if (src_index_this_buffer == src_wsabuf_p->len)
				{
					src_index_this_buffer = 0;
					++src_wsabuf_index;
				}
				dest_index_this_buffer += bytes_to_copy_this_step;
				TFG_ASSERT(dest_index_this_buffer <= dest_wsabuf_p->len);
				if (dest_index_this_buffer == dest_wsabuf_p->len)
				{
					dest_index_this_buffer = 0;
					++dest_wsabuf_index;
				}
			}
			TFG_ASSERT(dest_wsabuf_index <= dest_wsabuf_array_size);
			if (dest_wsabuf_index == dest_wsabuf_array_size)
			{
				m_CallbackIPtr->OnRecvPacket(this, dest_packet_p);
				dest_packet_p = 0;
				header_num_bytes_read = 0;
			}
		}
	}

	rv = S_OK;
finally:
	TFG_FUNC_EXIT("");
	return rv;
}


	}
}
