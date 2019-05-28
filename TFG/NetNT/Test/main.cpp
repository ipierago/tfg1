#include <TFG/NetNT/NetNT.h>

#define BUFFER_BLOCK_SIZE 512

int main()
{
    ::TFG::TFG s_TFG;

    TP_CALLBACK_ENVIRON tp_callback_environ;
    InitializeThreadpoolEnvironment(&tp_callback_environ);
    PTP_POOL ptp_pool = 0;
    ptp_pool = CreateThreadpool(NULL);
    if (ptp_pool)
    {
#if 0
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        uint32_t const max_thread_count = 2 * sysinfo.dwNumberOfProcessors;
#else
        uint32_t const max_thread_count = 6;
#endif
        SetThreadpoolThreadMaximum(ptp_pool, max_thread_count);
        SetThreadpoolCallbackPool(&tp_callback_environ, ptp_pool);
        {
            TFG::NetNT::NetNT_Init(&tp_callback_environ, 6, BUFFER_BLOCK_SIZE);
            // Do some stuff
            TFG::NetNT::NetNT_Deinit();
        }
        CloseThreadpool(ptp_pool);
        DestroyThreadpoolEnvironment(&tp_callback_environ);
    }

    return 0;
}
