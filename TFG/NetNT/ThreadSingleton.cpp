#include "NetNT.h"
#include "ThreadSingleton.h"
#include "Packet.h"
#include "Globals.h"

namespace TFG
{
namespace NetNT
{

ThreadSingleton::ThreadSingleton()
{
	g_net_globals.m_ObjectTrackerThreadSingleton.Add(this);
}

ThreadSingleton::~ThreadSingleton()
{
	g_net_globals.m_ObjectTrackerThreadSingleton.Remove(this);
}

void ThreadSingleton::Destroy()
{
	std::for_each(free_packets_std_ptrvec.begin(), free_packets_std_ptrvec.end(), [](Packet *p) { delete p; });
	std::for_each(free_buffers_std_ptrvec.begin(), free_buffers_std_ptrvec.end(), [](void *p) { delete[] p; });
	delete this;
}

ThreadSingleton &ThreadSingleton::Instance()
{
    static __declspec(thread) ThreadSingleton *s_ThreadSingletonPtr = 0;
    if (s_ThreadSingletonPtr == 0)
    {
        s_ThreadSingletonPtr = new ThreadSingleton;
    }
    return (*s_ThreadSingletonPtr);
}

} // namespace NetNT
} // namespace TFG
