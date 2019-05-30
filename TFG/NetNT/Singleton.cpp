#include "NetNT.h"
#include "Singleton.h"
#include "ThreadSingleton.h"

TFG_FILE_SETUP()

namespace TFG
{
namespace NetNT
{

Singleton Singleton::s_Instance;

Singleton::Singleton() : m_ObjectTrackerThreadSingleton(TFG_Level_Debug)
{
}

Singleton::~Singleton()
{
}

TFG_Result Singleton::Init(PTP_CALLBACK_ENVIRON const in_ptp_callback_environ, uint32_t const in_packet_buffer_size)
{
	TFG_FUNC_ENTER();
	TFG_Result rv = E_FAIL;

	ptp_callback_environ = in_ptp_callback_environ;
	packet_buffer_size = in_packet_buffer_size;

	rv = S_OK;
	//finally:
	if (FAILED(rv))
		Deinit();
	TFG_FUNC_EXIT("");
	return rv;
}

void Singleton::Deinit()
{
	TFG_FUNC_ENTER();

	m_ObjectTrackerUDPSocket.DestroyContents();
	m_ObjectTrackerThreadSingleton.DestroyContents();

	TFG_FUNC_EXIT("");
}

} // namespace NetNT
} // namespace TFG
