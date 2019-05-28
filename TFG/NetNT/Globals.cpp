#include "NetNT.h"
#include "Globals.h"

TFG_FILE_SETUP()

namespace TFG
{
namespace NetNT
{

Globals g_net_globals;

TFG_Result net_globals_init(GlobalsPtr const in_this, PTP_CALLBACK_ENVIRON const in_ptp_callback_environ, uint32_t const in_packet_buffer_size)
{
	TFG_FUNC_ENTER();
	TFG_Result rv = E_FAIL;

	memset(in_this, 0, sizeof(*in_this));

	in_this->ptp_callback_environ = in_ptp_callback_environ;
	in_this->packet_buffer_size = in_packet_buffer_size;

	rv = S_OK;
finally:
	if (FAILED(rv))
		net_globals_deinit(in_this);
	TFG_FUNC_EXIT("");
	return rv;
}

void net_globals_deinit(GlobalsPtr const in_this)
{
	TFG_FUNC_ENTER();

	TFG_FUNC_EXIT("");
}

} // namespace NetNT
} // namespace TFG
