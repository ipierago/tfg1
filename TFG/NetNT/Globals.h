#pragma once
#include "NetNT.h"

namespace TFG
{
namespace NetNT
{

class Globals
{
public:
	PTP_CALLBACK_ENVIRON ptp_callback_environ;
	uint32_t packet_buffer_size;
	HANDLE iocp_h;
	volatile int64_t num_packets_in_use;
};

extern Globals g_net_globals;

TFG_Result net_globals_init(GlobalsPtr const in_this, PTP_CALLBACK_ENVIRON const in_ptp_callback_environ, uint32_t const in_packet_buffer_size);
void net_globals_deinit(GlobalsPtr const in_this);

} // namespace NetNT
} // namespace TFG