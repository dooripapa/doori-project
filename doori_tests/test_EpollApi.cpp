//
// Created by jaeseong on 23. 5. 8.
//

#include <gtest/gtest.h>
#include <thread>
#include "Common/Log.h"
#include "Communication/TcpApi.h"
#include "Communication/EpollApi.h"

using namespace std;
using namespace doori::Common;
using namespace doori::Communication;

auto ProcessMessage(Socket socket) -> int
{
    string container;
    auto ret = socket.Recv(container, 10);

    if(ret==0)
    {
        LOG(DEBUG, "Socket closed");
    }

    LOG(DEBUG, "Recv[", container,"]");

    if(ret<0)
    {
        LOG(DEBUG, "error");
        return -1;
    }

    return ret;
}


static void RunClient() {

    Socket socket{};

    sleep(1);
    LOG( DEBUG, "RunClient ");

    TcpApi tcpApi{socket};

    tcpApi.InitEndpoint();
    ASSERT_TRUE(tcpApi.Status());

    tcpApi.Connect("127.0.0.1", "8888");
    ASSERT_TRUE(tcpApi.Status());

    auto i = 0;
    while( i++ < 5 )
    {
        auto ret = tcpApi.GetSocket().Send("1234567890");
        ASSERT_TRUE(ret > 0);
    }

    tcpApi.GetSocket().Close();
}

TEST(Epoll, Usage) {

    Socket socket{};

    TcpApi tcpApi{socket};

    tcpApi.InitEndpoint();
    ASSERT_TRUE(tcpApi.Status());

    tcpApi.SetReuseOpt("127.0.0.1", "8888");
    ASSERT_TRUE(tcpApi.Status());

    tcpApi.SetTimeoutOpt(5);
    ASSERT_TRUE(tcpApi.Status());

    tcpApi.Bind("127.0.0.1", "8888");
    ASSERT_TRUE(tcpApi.Status());

    tcpApi.Listen(10);
    ASSERT_TRUE(tcpApi.Status());

    auto listenSocket = tcpApi.GetSocket();

    EpollApi epollApi{tcpApi.GetSocket()};

    epollApi.InitEpoll();
    ASSERT_TRUE(epollApi.Status());

    /* 클라이언트 접속 프로그램 기동,
     * 단, 2초뒤에 기동됨.기동하자마자,
     * 바로, 접속시도를 막기위해서*/
    std::thread t(RunClient);

    epollApi.RunningForeground(10, 10, ProcessMessage);
}
