//
// Created by jaeseong on 23. 5. 8.
//

#include <gtest/gtest.h>
#include <thread>
#include "Common/Log.h"
#include "CommunicationMember/TcpApi.h"

using namespace std;
using namespace doori::Common;
using namespace doori::CommunicationMember;

auto ProcessMessage(TcpApi::Socket socket) -> int
{
    int iReadLen = 3;

    std::unique_ptr<char[]> dataContainer = std::make_unique<char []>(iReadLen+1);  // 1 is null size
    memset(dataContainer.get(), 0x00, iReadLen + 1);

    TcpApi::Recv(socket, dataContainer.get(), iReadLen);

    string stream;
    stream = dataContainer.get();
    LOG( DEBUG, "####### [", stream, "] #######");

    sleep(5);
    return 0;
}

static void RunClient() {
    sleep(2);
    LOG( DEBUG, "RunClient ");

    auto sock = TcpApi::CreateSocket();
    TcpApi::Connect(sock, "127.0.0.1", "8888");

    const char *p = "lee";
    TcpApi::Send(sock, p, 3);
}

TEST(Epoll, Usage) {

    auto socket = TcpApi::CreateSocket();

    TcpApi::Bind(socket, "127.0.0.1", "8888");

    TcpApi::Listen(socket, 10);

    auto epollFd = TcpApi::CreateEpoll(socket, 1);

    /* 클라이언트 접속 프로그램 기동,
     * 단, 2초뒤에 기동됨.기동하자마자,
     * 바로, 접속시도를 막기위해서*/
    std::thread t(RunClient);

    TcpApi::RunningEpoll(epollFd, socket, 1, 2000, ProcessMessage);

    t.join();

}
