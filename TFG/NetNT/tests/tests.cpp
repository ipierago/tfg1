#include <TFG/NetNT/NetNT.h>
#include <gtest/gtest.h>

#include <TFG/NetNT/Packet.h>
#include <TFG/NetNT/UDPSocket.h>

TFG_FILE_SETUP();

#define BUFFER_BLOCK_SIZE 512

int NetNT_Test_InitAndDeinit()
{
    int rv = -1;

    TFG_Init();
    TFG_SetGlobalLevelLog(TFG_Level_Debug);

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
            if (SUCCEEDED(TFG::NetNT::Init(&tp_callback_environ, 6, BUFFER_BLOCK_SIZE)))
            {
                // Do some stuff

                rv = 0;
                TFG::NetNT::Deinit();
            }
        }
        CloseThreadpool(ptp_pool);
        DestroyThreadpoolEnvironment(&tp_callback_environ);
    }

    TFG_Deinit();

    return rv;
}

#if 0
TEST(NetNT_Test_Standalone, InitAndDeinit)
{
    EXPECT_EQ(NetNT_Test_InitAndDeinit(), 0);
}
#endif

class NetNT_Test : public testing::Test
{
public:
    TP_CALLBACK_ENVIRON tp_callback_environ;
    PTP_POOL ptp_pool;

    virtual void SetUp()
    {
        TFG_Init();
        TFG_SetGlobalLevelLog(TFG_Level_Debug);

        InitializeThreadpoolEnvironment(&tp_callback_environ);
        ptp_pool = CreateThreadpool(NULL);

#if 0
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        uint32_t const max_thread_count = 2 * sysinfo.dwNumberOfProcessors;
#else
        uint32_t const max_thread_count = 6;
#endif
        SetThreadpoolThreadMaximum(ptp_pool, max_thread_count);
        SetThreadpoolCallbackPool(&tp_callback_environ, ptp_pool);
        TFG_Result const r = TFG::NetNT::Init(&tp_callback_environ, 6, BUFFER_BLOCK_SIZE);
    }
    virtual void TearDown()
    {
        TFG::NetNT::Deinit();
        if (ptp_pool != 0)
        {
            CloseThreadpool(ptp_pool);
            ptp_pool = 0;
        }
        DestroyThreadpoolEnvironment(&tp_callback_environ);
        TFG_Deinit();
    }

    class MyUDPSocketCallback : public TFG::NetNT::UDPSocket::CallbackI
    {
    public:
        virtual void OnRecvPacket(TFG::NetNT::UDPSocketPtr const in_net_udpsocket_p, TFG::NetNT::PacketPtr const in_net_packet_p)
        {
            UNREFERENCED_PARAMETER(in_net_udpsocket_p);
            in_net_packet_p->Release();
        };
        virtual void OnSendComplete(TFG::NetNT::UDPSocketPtr const in_net_udpsocket_p, TFG::NetNT::PacketPtr const in_net_packet_p, TFG_Result const in_result)
        {
            UNREFERENCED_PARAMETER(in_net_udpsocket_p);
            UNREFERENCED_PARAMETER(in_result);
            in_net_packet_p->Release();
        };
        virtual void OnError(TFG::NetNT::UDPSocketPtr const in_net_udpsocket_p, TFG_Result const in_hr)
        {
            UNREFERENCED_PARAMETER(in_net_udpsocket_p);
            UNREFERENCED_PARAMETER(in_hr);
        }
    } myUDPSocketCallback;
};

#if 0
TEST_F(NetNT_Test, Packet)
{
    TFG::NetNT::PacketPtr const packetPtr = TFG::NetNT::Packet::Create();
    GTEST_ASSERT_NE(packetPtr, (TFG::NetNT::PacketPtr)0);

    char const *const stringPtr = "this is a test";
    uint32_t const stringLen = strlen(stringPtr);
    uint32_t const charArraySize = stringLen + 1;

    // AppendData
    uint32_t const rvAppendData = packetPtr->AppendData(stringPtr, charArraySize);
    EXPECT_EQ(rvAppendData, charArraySize);

    // GetData
    char *const pszGetData = new char[charArraySize];
    uint32_t const rvGetData = packetPtr->GetData(pszGetData, charArraySize);
    EXPECT_EQ(rvGetData, charArraySize);
    int const rvGetData_memcmp = memcmp(stringPtr, pszGetData, charArraySize);
    EXPECT_EQ(rvGetData_memcmp, 0);
    delete[] pszGetData;

    // Release
    int32_t const rvRelease = packetPtr->Release();
    EXPECT_EQ(rvRelease, 0);
}
#endif

#if 1
TEST_F(NetNT_Test, UDPSocket)
{
    TFG::NetNT::UDPSocketPtr const udpSocketPtr = TFG::NetNT::UDPSocket::Create("0.0.0.0", 4002, &myUDPSocketCallback, 1, 512);
    GTEST_ASSERT_NE(udpSocketPtr, (TFG::NetNT::UDPSocketPtr)0);

    udpSocketPtr->Destroy();
}
#endif

#if 0
TEST_F(NetNT_Test, UDPSocket_Loopback)
{
    class MyUDPSocketCallback : public TFG::NetNT::UDPSocket::CallbackI
    {
    public:
        virtual void OnRecvPacket(TFG::NetNT::UDPSocketPtr const in_net_udpsocket_p, TFG::NetNT::PacketPtr const in_net_packet_p)
        {
            UNREFERENCED_PARAMETER(in_net_udpsocket_p);

            uint32_t const size = in_net_packet_p->ComputeSize();
            char *const charArray = new char[size];
            uint32_t const rvGetData = in_net_packet_p->GetData(charArray, size);
            EXPECT_EQ(rvGetData, size);
            int const rvGetData_memcmp = memcmp(stringPtr, charArray, size);
            EXPECT_EQ(rvGetData_memcmp, 0);

            delete[] charArray;

            in_net_packet_p->Release();
            m_result = S_OK;
            SetEvent(m_EventHandle);
        };
        virtual void OnSendComplete(TFG::NetNT::UDPSocketPtr const in_net_udpsocket_p, TFG::NetNT::PacketPtr const in_net_packet_p, TFG_Result const in_result)
        {
            UNREFERENCED_PARAMETER(in_net_udpsocket_p);
            in_net_packet_p->Release();
            if (FAILED(in_result))
            {
                m_result = in_result;
                SetEvent(m_EventHandle);
            }
        };
        virtual void OnError(TFG::NetNT::UDPSocketPtr const in_net_udpsocket_p, TFG_Result const in_hr)
        {
            UNREFERENCED_PARAMETER(in_net_udpsocket_p);
            if (FAILED(in_hr))
            {
                m_result = in_hr;
                SetEvent(m_EventHandle);
            }
        }

        HANDLE m_EventHandle;
        TFG_Result m_result;
        char const *const stringPtr = "this is a test";

        MyUDPSocketCallback() : m_result(E_FAIL), m_EventHandle(0)
        {
            m_EventHandle = CreateEvent(
                NULL,               // default security attributes
                TRUE,               // manual-reset event
                FALSE,              // initial state is nonsignaled
                TEXT("EventHandle") // object name
            );
        }
        ~MyUDPSocketCallback()
        {
            CloseHandle(m_EventHandle);
        }

    } myUDPSocketCallback;

    TFG::NetNT::UDPSocketPtr const udpSocketPtr = TFG::NetNT::UDPSocket::Create("127.0.0.1", 4003, &myUDPSocketCallback, 1, 512);
    GTEST_ASSERT_NE(udpSocketPtr, (TFG::NetNT::UDPSocketPtr)0);

    TFG::NetNT::PacketPtr const packetPtr = TFG::NetNT::Packet::Create();
    GTEST_ASSERT_NE(packetPtr, (TFG::NetNT::PacketPtr)0);

    char const *const stringPtr = myUDPSocketCallback.stringPtr;
    uint32_t const stringLen = strlen(stringPtr);
    uint32_t const charArraySize = stringLen + 1;

    // AppendData
    uint32_t const rvAppendData = packetPtr->AppendData(stringPtr, charArraySize);
    EXPECT_EQ(rvAppendData, charArraySize);

    struct sockaddr_in remote_sockaddr_in;
    TFG::NetNT::InitSockAddrIn(&remote_sockaddr_in, "127.0.0.1", 4003);
    TFG_Result const rSendPacket = udpSocketPtr->SendPacket(packetPtr, &remote_sockaddr_in);
    EXPECT_EQ(rSendPacket, S_OK);
    int32_t const rvRelease = packetPtr->Release();

    WaitForSingleObject(myUDPSocketCallback.m_EventHandle, 10000);
    EXPECT_EQ(myUDPSocketCallback.m_result, S_OK);

    udpSocketPtr->Destroy();
}
#endif
