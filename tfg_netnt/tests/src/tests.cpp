#include <TFG/NetNT/NetNT.h>
#include <gtest/gtest.h>

#include <TFG/NetNT/Packet.h>
#include <TFG/NetNT/UDPSocket.h>
#include <TFG/NetNT/TCPAcceptor.h>
#include <TFG/NetNT/TCPConnector.h>
#include <TFG/NetNT/TCPConnection.h>
#include <TFG/NetNT/StreamToDGram.h>

TFG_FILE_SETUP();

#define BUFFER_BLOCK_SIZE 512

#if 0
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
#endif

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
#if 0
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
#endif
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

#if 0
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

#if 0
TEST_F(NetNT_Test, TCPAcceptor)
{
	class MyTCPAcceptorCallback : public TFG::NetNT::TCPAcceptor::CallbackI
	{
	public:
		virtual void OnConnectionAttempted(TFG::NetNT::TCPAcceptorPtr const in_net_tcpacceptor_p, TFG::NetNT::TCPConnection::CallbackI * * const out_net_tcpconnection_callback_i_p, uint32_t * const out_receive_buffer_size_p, void * * const out_context_pv)
		{
		}
		virtual void OnConnectionSucceeded(TFG::NetNT::TCPAcceptorPtr const in_net_tcpacceptor_p, TFG::NetNT::TCPConnectionPtr const in_net_tcpconnection_p)
		{

		}
		virtual void OnError(TFG::NetNT::TCPAcceptorPtr const in_net_tcpacceptor_p, TFG_Result const in_hr)
		{

		}
	} myTCPAcceptorCallback;

	TFG::NetNT::TCPAcceptorPtr const tcpAcceptorPtr = TFG::NetNT::TCPAcceptor::Create("127.0.0.1", 4004, &myTCPAcceptorCallback);
	GTEST_ASSERT_NE(tcpAcceptorPtr, (TFG::NetNT::TCPAcceptorPtr)0);

	tcpAcceptorPtr->Destroy();
}
#endif

#if 0
TEST_F(NetNT_Test, TCPAcceptor2)
{
	class MyTCPAcceptorCallback : public TFG::NetNT::TCPAcceptor::CallbackI
	{
	public:
		virtual void OnConnectionAttempted(TFG::NetNT::TCPAcceptorPtr const in_net_tcpacceptor_p, TFG::NetNT::TCPConnection::CallbackI * * const out_net_tcpconnection_callback_i_p, uint32_t * const out_receive_buffer_size_p, void * * const out_context_pv)
		{
		}
		virtual void OnConnectionSucceeded(TFG::NetNT::TCPAcceptorPtr const in_net_tcpacceptor_p, TFG::NetNT::TCPConnectionPtr const in_net_tcpconnection_p)
		{

		}
		virtual void OnError(TFG::NetNT::TCPAcceptorPtr const in_net_tcpacceptor_p, TFG_Result const in_hr)
		{

		}
	} myTCPAcceptorCallback;

	TFG::NetNT::TCPAcceptorPtr const tcpAcceptorPtr = TFG::NetNT::TCPAcceptor::Create("127.0.0.1", 4004, &myTCPAcceptorCallback);
	GTEST_ASSERT_NE(tcpAcceptorPtr, (TFG::NetNT::TCPAcceptorPtr)0);

	tcpAcceptorPtr->Destroy();
}
#endif

#if 0
TEST_F(NetNT_Test, TCP_Loopback)
{
    class MyTCPConnectionCallback : public TFG::NetNT::TCPConnection::CallbackI
    {
    public:
        virtual void OnRecvPacket(TFG::NetNT::TCPConnectionPtr const in_net_tcpconnection_p, TFG::NetNT::PacketPtr const in_net_packet_p)
        {
            TFG_DEBUG("");
        };
        virtual void OnSendComplete(TFG::NetNT::TCPConnectionPtr const in_net_tcpconnection_p, TFG::NetNT::PacketPtr const in_net_packet_p, TFG_Result const in_result)
        {
            TFG_DEBUG("");
        };
        virtual void OnError(TFG::NetNT::TCPConnectionPtr const in_net_tcpconnection_p, TFG_Result const in_hr)
        {
            TFG_DEBUG("");
        };
    } myTCPConnectionCallback;

    class MyTCPAcceptorCallback : public TFG::NetNT::TCPAcceptor::CallbackI
    {
        MyTCPConnectionCallback &m_myTCPConnectionCallback;
        std::vector<TFG::NetNT::TCPConnectionPtr> m_tcpConnectionPtrArray;

    public:
        MyTCPAcceptorCallback(MyTCPConnectionCallback &myTCPConnectionCallback) : m_myTCPConnectionCallback(myTCPConnectionCallback)
        {
        }
        ~MyTCPAcceptorCallback()
        {
            std::for_each(m_tcpConnectionPtrArray.begin(), m_tcpConnectionPtrArray.end(), [](TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr) {TFG_DEBUG("");  tcpConnectionPtr->Destroy(); });
            m_tcpConnectionPtrArray.resize(0);
        }
        virtual void OnConnectionAttempted(TFG::NetNT::TCPAcceptorPtr const tcpAcceptorPtr, TFG::NetNT::TCPConnection::CallbackI **const tcpConnectorCallbackPtrPtr, uint32_t *const recvBufSizePtr, void **const contextPtrPtr)
        {
            TFG_FUNC_ENTER();
            *tcpConnectorCallbackPtrPtr = &m_myTCPConnectionCallback;
            *recvBufSizePtr = 512;
            *contextPtrPtr = 0;
            TFG_FUNC_EXIT("");
        }
        virtual void OnConnectionSucceeded(TFG::NetNT::TCPAcceptorPtr const tcpAcceptorPtr, TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr)
        {
            m_tcpConnectionPtrArray.push_back(tcpConnectionPtr);
            TFG_DEBUG("");
        }
        virtual void OnError(TFG::NetNT::TCPAcceptorPtr const in_net_tcpacceptor_p, TFG_Result const in_hr)
        {
            TFG_DEBUG("");
        }
    } myTCPAcceptorCallback(myTCPConnectionCallback);

    class MyTCPConnectorCallback : public TFG::NetNT::TCPConnector::CallbackI
    {
        MyTCPConnectionCallback m_myTCPConnectionCallback;
        std::vector<TFG::NetNT::TCPConnectionPtr> m_tcpConnectionPtrArray;

    public:
        MyTCPConnectorCallback(MyTCPConnectionCallback &myTCPConnectionCallback) : m_myTCPConnectionCallback(myTCPConnectionCallback) {}
        ~MyTCPConnectorCallback()
        {
            std::for_each(m_tcpConnectionPtrArray.begin(), m_tcpConnectionPtrArray.end(), [](TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr) {TFG_DEBUG("");  tcpConnectionPtr->Destroy(); });
            m_tcpConnectionPtrArray.resize(0);
        }
        virtual void OnConnectionAttempted(TFG::NetNT::TCPConnectorPtr const tcpConnectorPtr, TFG::NetNT::TCPConnection::CallbackI **const tcpConnectionCallbackIPtrPtr, uint32_t *const recvBufSizePtr, void **const contextPtrPtr)
        {
            TFG_FUNC_ENTER();
            *tcpConnectionCallbackIPtrPtr = &m_myTCPConnectionCallback;
            *recvBufSizePtr = 512;
            *contextPtrPtr = 0;
            TFG_FUNC_EXIT("");
        }
        virtual void OnConnectionSucceeded(TFG::NetNT::TCPConnectorPtr const tcpConnectorPtr, TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr)
        {
            m_tcpConnectionPtrArray.push_back(tcpConnectionPtr);
            TFG_DEBUG("");
        }
        virtual void OnError(TFG::NetNT::TCPConnectorPtr const in_net_tcpconnector_p, TFG_Result const in_hr)
        {
            TFG_DEBUG("");
        }
    } myTCPConnectorCallback(myTCPConnectionCallback);

    TFG::NetNT::TCPAcceptorPtr const tcpAcceptorPtr = TFG::NetNT::TCPAcceptor::Create("127.0.0.1", 4004, &myTCPAcceptorCallback);
    GTEST_ASSERT_NE(tcpAcceptorPtr, (TFG::NetNT::TCPAcceptorPtr)0);

    TFG::NetNT::TCPConnectorPtr const tcpConnectorPtr = TFG::NetNT::TCPConnector::Create("127.0.0.1", 4005, "127.0.0.1", 4004, &myTCPConnectorCallback);
    GTEST_ASSERT_NE(tcpConnectorPtr, (TFG::NetNT::TCPConnectorPtr)0);

    printf("Test is running.  Press a key to terminate...\n");
    _getch();
    //Sleep(1500);

    tcpConnectorPtr->Destroy();

    tcpAcceptorPtr->Destroy();
}
#endif

struct IPAndPort {
	char m_IP[16];
	unsigned int m_Port;
};

IPAndPort IPAndPortFromSockAddrIn(struct sockaddr_in & sockAddrIn)
{
	IPAndPort rv;
	inet_ntop(AF_INET, &sockAddrIn.sin_addr, rv.m_IP, sizeof(rv.m_IP));
	rv.m_Port = ntohs(sockAddrIn.sin_port);
	return rv;
}

IPAndPort IPAndPortFromSocket(SOCKET sockfd) {
	struct sockaddr_in my_addr;
	memset(&my_addr, 0, sizeof(my_addr));
	int len = sizeof(my_addr);
	getsockname(sockfd, (struct sockaddr *) &my_addr, &len);
	return IPAndPortFromSockAddrIn(my_addr);
}

#if 1
TEST_F(NetNT_Test, TCP_LoopbackEchoSimple)
{
    class TCPEchoServer : public TFG::NetNT::TCPConnection::CallbackI, TFG::NetNT::TCPAcceptor::CallbackI
    {
    public:
        TCPEchoServer(int const port = 0) : m_tcpAcceptorPtr(0)
        {
            m_tcpAcceptorPtr = TFG::NetNT::TCPAcceptor::Create("127.0.0.1", port, this);
			IPAndPort ipAndPortAcceptorSocket = IPAndPortFromSocket(m_tcpAcceptorPtr->GetSocket());
			IPAndPort ipAndPortAcceptorAcceptSocket = IPAndPortFromSocket(m_tcpAcceptorPtr->GetAcceptSocket());
			TFG_DEBUG("Acceptor::Socket %s:%u, Acceptor::AcceptSocket %s:%u", ipAndPortAcceptorSocket.m_IP, ipAndPortAcceptorSocket.m_Port, ipAndPortAcceptorAcceptSocket.m_IP, ipAndPortAcceptorAcceptSocket.m_Port);
        }
        ~TCPEchoServer()
        {
            std::for_each(m_tcpConnectionPtrArray.begin(), m_tcpConnectionPtrArray.end(), [](TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr) { tcpConnectionPtr->Destroy(); });
            m_tcpConnectionPtrArray.resize(0);
            m_tcpAcceptorPtr->Destroy();
            m_tcpAcceptorPtr = 0;
        }
        virtual void TFG::NetNT::TCPConnection::CallbackI::OnRecvPacket(TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr, TFG::NetNT::PacketPtr const packetPtr)
        {
			IPAndPort ipAndPortConnection = IPAndPortFromSocket(tcpConnectionPtr->GetSocket());
			IPAndPort ipAndPortPacket = IPAndPortFromSockAddrIn(*packetPtr->GetSockAddrInPtr());
			TFG_DEBUG("tcpConnectionPtr: %s:%u, packetPtr: %s:%u", ipAndPortConnection.m_IP, ipAndPortConnection.m_Port, ipAndPortPacket.m_IP, ipAndPortPacket.m_Port);
			tcpConnectionPtr->SendPacket(packetPtr);
        };
        virtual void TFG::NetNT::TCPConnection::CallbackI::OnSendComplete(TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr, TFG::NetNT::PacketPtr const packetPtr, TFG_Result const in_result)
        {
			IPAndPort ipAndPortConnection = IPAndPortFromSocket(tcpConnectionPtr->GetSocket());
			IPAndPort ipAndPortPacket = IPAndPortFromSockAddrIn(*packetPtr->GetSockAddrInPtr());
			TFG_DEBUG("tcpConnectionPtr: %s:%u, packetPtr: %s:%u", ipAndPortConnection.m_IP, ipAndPortConnection.m_Port, ipAndPortPacket.m_IP, ipAndPortPacket.m_Port);
        };
        virtual void TFG::NetNT::TCPConnection::CallbackI::OnError(TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr, TFG_Result const in_hr)
        {
			IPAndPort ipAndPortConnection = IPAndPortFromSocket(tcpConnectionPtr->GetSocket());
			TFG_DEBUG("tcpConnectionPtr: %s:%u, in_hr: 0x%08lx (%s)", ipAndPortConnection.m_IP, ipAndPortConnection.m_Port, in_hr, TFG_GetErrorString(in_hr));
        };

        virtual void TFG::NetNT::TCPAcceptor::CallbackI::OnConnectionAttempted(TFG::NetNT::TCPAcceptorPtr const tcpAcceptorPtr, TFG::NetNT::TCPConnection::CallbackI **const tcpConnectorCallbackPtrPtr, uint32_t *const recvBufSizePtr, void **const contextPtrPtr)
        {
            TFG_FUNC_ENTER();
			IPAndPort ipAndPort = IPAndPortFromSocket(tcpAcceptorPtr->GetSocket());
			TFG_DEBUG("%s:%u", ipAndPort.m_IP, ipAndPort.m_Port);
            *tcpConnectorCallbackPtrPtr = this;
            *recvBufSizePtr = 512;
            *contextPtrPtr = 0;
            TFG_FUNC_EXIT("");
        }
        virtual void TFG::NetNT::TCPAcceptor::CallbackI::OnConnectionSucceeded(TFG::NetNT::TCPAcceptorPtr const tcpAcceptorPtr, TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr)
        {
			IPAndPort ipAndPortAcceptor = IPAndPortFromSocket(tcpAcceptorPtr->GetSocket());
			IPAndPort ipAndPortConnection = IPAndPortFromSocket(tcpConnectionPtr->GetSocket());
			TFG_DEBUG("Acceptor: %s:%u, Connection: %s:%u", ipAndPortAcceptor.m_IP, ipAndPortAcceptor.m_Port, ipAndPortConnection.m_IP, ipAndPortConnection.m_Port);
            m_tcpConnectionPtrArray.push_back(tcpConnectionPtr);
        }
        virtual void TFG::NetNT::TCPAcceptor::CallbackI::OnError(TFG::NetNT::TCPAcceptorPtr const tcpAcceptorPtr, TFG_Result const in_hr)
        {
			IPAndPort ipAndPortAcceptor = IPAndPortFromSocket(tcpAcceptorPtr->GetSocket());
			TFG_DEBUG("tcpAcceptorPtr: %s:%u, in_hr: 0x%08lx (%s)", ipAndPortAcceptor.m_IP, ipAndPortAcceptor.m_Port, in_hr, TFG_GetErrorString(in_hr));
        }

		TFG::NetNT::TCPAcceptorPtr GetTCPAcceptorPtr() { return m_tcpAcceptorPtr; }

    private:
        TFG::NetNT::TCPAcceptorPtr m_tcpAcceptorPtr;
        std::vector<TFG::NetNT::TCPConnectionPtr> m_tcpConnectionPtrArray;
    };

    class TCPEchoClient : public TFG::NetNT::TCPConnector::CallbackI, TFG::NetNT::TCPConnection::CallbackI
    {
    public:
		TCPEchoClient(int const remotePort)
        {
            m_tcpConnectorPtr = TFG::NetNT::TCPConnector::Create(INADDR_ANY, 0, "127.0.0.1", remotePort, this);
			IPAndPort ipAndPortConnector = IPAndPortFromSocket(m_tcpConnectorPtr->GetSocket());
			TFG_DEBUG("Connector: %s:%u", ipAndPortConnector.m_IP, ipAndPortConnector.m_Port);
        }
        ~TCPEchoClient()
        {
            std::for_each(m_tcpConnectionPtrArray.begin(), m_tcpConnectionPtrArray.end(), [](TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr) {tcpConnectionPtr->Destroy(); });
            m_tcpConnectionPtrArray.resize(0);
            m_tcpConnectorPtr->Destroy();
            m_tcpConnectorPtr = 0;
        }

		// TCPConnector::CallbackI
        virtual void TFG::NetNT::TCPConnector::CallbackI::OnConnectionAttempted(TFG::NetNT::TCPConnectorPtr const tcpConnectorPtr, TFG::NetNT::TCPConnection::CallbackI **const tcpConnectionCallbackIPtrPtr, uint32_t *const recvBufSizePtr, void **const contextPtrPtr)
        {
            TFG_FUNC_ENTER();
			IPAndPort ipAndPortConnector = IPAndPortFromSocket(tcpConnectorPtr->GetSocket());
			TFG_DEBUG("Connector: %s:%u", ipAndPortConnector.m_IP, ipAndPortConnector.m_Port);
            *tcpConnectionCallbackIPtrPtr = this;
            *recvBufSizePtr = 512;
            *contextPtrPtr = 0;
            TFG_FUNC_EXIT("");
        }
        virtual void TFG::NetNT::TCPConnector::CallbackI::OnConnectionSucceeded(TFG::NetNT::TCPConnectorPtr const tcpConnectorPtr, TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr)
        {
			IPAndPort ipAndPortConnector= IPAndPortFromSocket(tcpConnectorPtr->GetSocket());
			IPAndPort ipAndPortConnection = IPAndPortFromSocket(tcpConnectionPtr->GetSocket());
			TFG_DEBUG("Connector: %s:%u, Connection: %s:%u", ipAndPortConnector.m_IP, ipAndPortConnector.m_Port, ipAndPortConnection.m_IP, ipAndPortConnection.m_Port);
			m_tcpConnectionPtrArray.push_back(tcpConnectionPtr);

			// Send test packet
			TFG::NetNT::PacketPtr const packetPtr = TFG::NetNT::Packet::Create();
			char const *const stringPtr = "this is a test";
			uint32_t const stringLen = (uint32_t)strlen(stringPtr);
			uint32_t const charArraySize = stringLen + 1;
			uint32_t const rvAppendData = packetPtr->AppendData(stringPtr, charArraySize);
			TFG_Result const rSendPacket = tcpConnectionPtr->SendPacket(packetPtr);
			EXPECT_EQ(rSendPacket, S_OK);
			int32_t const rvRelease = packetPtr->Release();
        }
        virtual void TFG::NetNT::TCPConnector::CallbackI::OnError(TFG::NetNT::TCPConnectorPtr const tcpConnectorPtr, TFG_Result const in_hr)
        {
			IPAndPort ipAndPortConnector = IPAndPortFromSocket(tcpConnectorPtr->GetSocket());
			TFG_DEBUG("tcpConnectorPtr: %s:%u, in_hr: 0x%08lx (%s)", ipAndPortConnector.m_IP, ipAndPortConnector.m_Port, in_hr, TFG_GetErrorString(in_hr));
        }

		// TCPConnection::CallbackI
		virtual void TFG::NetNT::TCPConnection::CallbackI::OnRecvPacket(TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr, TFG::NetNT::PacketPtr const packetPtr)
		{
			IPAndPort ipAndPortConnection = IPAndPortFromSocket(tcpConnectionPtr->GetSocket());
			IPAndPort ipAndPortPacket = IPAndPortFromSockAddrIn(*packetPtr->GetSockAddrInPtr());
			TFG_DEBUG("tcpConnectionPtr: %s:%u, packetPtr: %s:%u", ipAndPortConnection.m_IP, ipAndPortConnection.m_Port, ipAndPortPacket.m_IP, ipAndPortPacket.m_Port);
		};
		virtual void TFG::NetNT::TCPConnection::CallbackI::OnSendComplete(TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr, TFG::NetNT::PacketPtr const packetPtr, TFG_Result const in_result)
		{
			IPAndPort ipAndPortConnection = IPAndPortFromSocket(tcpConnectionPtr->GetSocket());
			IPAndPort ipAndPortPacket = IPAndPortFromSockAddrIn(*packetPtr->GetSockAddrInPtr());
			TFG_DEBUG("tcpConnectionPtr: %s:%u, packetPtr: %s:%u", ipAndPortConnection.m_IP, ipAndPortConnection.m_Port, ipAndPortPacket.m_IP, ipAndPortPacket.m_Port);
		};
		virtual void TFG::NetNT::TCPConnection::CallbackI::OnError(TFG::NetNT::TCPConnectionPtr const tcpConnectionPtr, TFG_Result const in_hr)
		{
			IPAndPort ipAndPortConnection = IPAndPortFromSocket(tcpConnectionPtr->GetSocket());
			TFG_DEBUG("tcpConnectionPtr: %s:%u, in_hr: 0x%08lx (%s)", ipAndPortConnection.m_IP, ipAndPortConnection.m_Port, in_hr, TFG_GetErrorString(in_hr));
		};


    private:
        TFG::NetNT::TCPConnectorPtr m_tcpConnectorPtr;
        std::vector<TFG::NetNT::TCPConnectionPtr> m_tcpConnectionPtrArray;
    };

    TCPEchoServer tcpEchoServer;
	IPAndPort ipAndPortAcceptorSocket = IPAndPortFromSocket(tcpEchoServer.GetTCPAcceptorPtr()->GetSocket());
    TCPEchoClient tcpEchoClient0(ipAndPortAcceptorSocket.m_Port);
	TCPEchoClient tcpEchoClient1(ipAndPortAcceptorSocket.m_Port);

    Sleep(1000);
}
#endif
