//
// Created by jaeseong on 23. 5. 30.
//

#include <gtest/gtest.h>
#include <thread>
#include "Communication/TcpApi.h"
#include "Communication/Socket.h"

using namespace doori::api::Communication;

void TcpApiClient() {

    sleep(5);

    Socket socket{};

    TcpApi tcpApi{socket};

    tcpApi.InitEndpoint();
    ASSERT_TRUE(tcpApi.Status()) << "TcpApiClient InitEndpoint() error" ;

    tcpApi.Connect("127.0.0.1", "7777");
    ASSERT_TRUE(tcpApi.Status()) << "TcpApiClient Connect() error" ;

    auto connectedSocket = tcpApi.GetSocket();

    connectedSocket.Send("1234567890");
}

TEST(TcpApi, Usage) {

    Socket socket{};

    TcpApi tcpApi{socket};

    tcpApi.InitEndpoint();
    ASSERT_TRUE(tcpApi.Status()) << "InitEndpoint() error" ;

    tcpApi.SetReuseOpt("127.0.0.1", "7777");
    ASSERT_TRUE(tcpApi.Status()) << "SetReuseOpt() error" ;

    tcpApi.SetTimeoutOpt(1);
    ASSERT_TRUE(tcpApi.Status()) << "SetTimeoutOpt() error" ;

    tcpApi.Bind("127.0.0.1", "7777");
    ASSERT_TRUE(tcpApi.Status()) << "Bind() error" ;

    tcpApi.Listen(10);
    ASSERT_TRUE(tcpApi.Status()) << "Listen() error" ;

    /* 클라이언트에 소켓 접속시도*/
    std::thread t(TcpApiClient);

    auto fd = tcpApi.Accept();
    ASSERT_TRUE(tcpApi.Status()) << "Accept() error" ;

    auto ret = tcpApi.GetSocket();
    ASSERT_FALSE(ret.GetFd() == fd ) << "Accept() fd:"<< fd << "  Socket fd:" << ret.GetFd();


    Socket acceptSocket{fd, SOCK_STATUS::ESTABLISED};
    string container;
    acceptSocket.Recv(container, 10);

    EXPECT_EQ(container, "1234567890") << "recv data is wrong";

    t.join();
}