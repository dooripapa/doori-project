//
// Created by jaeseong on 23. 5. 8.
//

#include <gtest/gtest.h>
#include <thread>
#include "Common/Log.h"
#include "CommunicationMember/TcpApi.h"
#include "CommunicationMember/Epoll.h"
#include "CommunicationMember/EpollEvents.h"

using namespace std;
using namespace doori::Common;
using namespace doori::CommunicationMember;

auto ProcessMessage(int socket, string& stream) -> int
{
    int iReadLen = 0;

    std::unique_ptr<char[]> dataContainer = std::make_unique<char []>(iReadLen+1);  // 1 is null size
    memset(dataContainer.get(), 0x00, iReadLen + 1);

    TcpApi::Recv(socket, dataContainer.get(), iReadLen);

    stream = dataContainer.get()
    LOG( DEBUG, "####### [", stream, "] #######");

    return 0;
}

void RunClient() {
    LOG( DEBUG, "RunClient ");
}

TEST(CommunicationMember, Usage)
{
    EpollEvents events;
    events.setSize(100);

    Epoll epoll(ProcessMessage);

    /* 클라이언트 접속 프로그램 기동,
     * 단, 2초뒤에 기동됨.기동하자마자,
     * 바로, 접속시도를 막기위해서*/
    std::thread t(RunClient);

    /* 클라이언트 종료대기 */
    t.join();
}
