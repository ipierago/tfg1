#pragma once
#include "NetNT.h"
#include "ObjectTracker.h"

namespace TFG
{
namespace NetNT
{

class Singleton
{
public:
	PTP_CALLBACK_ENVIRON ptp_callback_environ;
	uint32_t packet_buffer_size;
	HANDLE iocp_h;
	volatile int64_t num_packets_in_use;

	ObjectTracker<ThreadSingleton> m_ObjectTrackerThreadSingleton;
	ObjectTracker<UDPSocket> m_ObjectTrackerUDPSocket;
	ObjectTracker<TCPConnection> m_ObjectTrackerTCPConnection;
	ObjectTracker<TCPConnector> m_ObjectTrackerTCPConnector;
	ObjectTracker<TCPAcceptor> m_ObjectTrackerTCPAcceptor;

	TFG_Result Init(PTP_CALLBACK_ENVIRON const in_ptp_callback_environ, uint32_t const in_packet_buffer_size);
	void Deinit();

	static Singleton &Get() { return s_Instance; }

private:
	Singleton();
	~Singleton();

	static Singleton s_Instance;
};

} // namespace NetNT
} // namespace TFG
