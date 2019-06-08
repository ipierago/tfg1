#include "stdafx.h"
#include "netnt.h"
#include "threadsingleton.h"
#include "packet.h"
#include "singleton.h"

namespace TFG
{
namespace NetNT
{

ThreadSingleton::ThreadSingleton()
{
    Singleton::Get().m_ObjectTrackerThreadSingleton.Add(this);
}

ThreadSingleton::~ThreadSingleton()
{
    Singleton::Get().m_ObjectTrackerThreadSingleton.Remove(this);
}

void ThreadSingleton::Destroy()
{
    std::for_each(free_packets_std_ptrvec.begin(), free_packets_std_ptrvec.end(), [](Packet *p) { delete p; });
	free_packets_std_ptrvec.resize(0);
    std::for_each(free_buffers_std_ptrvec.begin(), free_buffers_std_ptrvec.end(), [](void *p) { delete[] p; });
	free_buffers_std_ptrvec.resize(0);
}

ThreadSingleton * & ThreadSingleton::InstancePtr()
{
	static __declspec(thread) ThreadSingleton *s_ThreadSingletonPtr = 0;
	return s_ThreadSingletonPtr;
}

ThreadSingleton &ThreadSingleton::Instance()
{
	static __declspec(thread) ThreadSingleton threadSingleton;
    return threadSingleton;
}

} // namespace NetNT
} // namespace TFG
