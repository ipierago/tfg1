#pragma once
#include "NetNT.h"

namespace TFG
{

namespace NetNT
{

class ThreadSingleton
{
public:
    std::vector<Packet *> free_packets_std_ptrvec;
    std::vector<void *> free_buffers_std_ptrvec;
    static ThreadSingleton &Instance();
	void Destroy();

private:
	ThreadSingleton();
	~ThreadSingleton();
};

} // namespace NetNT

} // namespace TFG
