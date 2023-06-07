//
// Created by jaeseong on 23. 2. 2.
//
#include <gtest/gtest.h>
#include <thread>
#include "Communication/TCPBuilder.h"
#include "Common/Log.h"
#include "Communication/TCP.h"

using namespace doori::Communication;
using namespace doori::Common;
using namespace std;

void RunClient() {

    Socket socket{};

    std::unique_ptr<IIPC> iipc = std::make_unique<TCP>();

    TCPBuilder builder = TCPBuilder(socket);

    builder.To("127.0.0.1", "8888");

    try {
        sleep(2);

        iipc->Create( builder );
    }catch (std::exception e) {
        cout << e.what() << endl;
    }

    string container;

    auto conn = builder.GetTopology();

    sleep(5);

    conn->Send("Hello doori world.");

    conn->Recv(container, 18);

    LOG(DEBUG, "Client Recv:", container);
}

TEST(CommunicationMember, Usage) {

    Socket socket{};

    std::unique_ptr<IIPC> iipc = std::make_unique<TCP>();

    TCPBuilder builder = TCPBuilder(socket);

    builder.From("127.0.0.1", "8888");

    /* 클라이언트 접속 프로그램 기동,
     * 단, 2초뒤에 기동됨.기동하자마자,
     * 바로, 접속시도를 막기위해서*/
    std::thread t(RunClient);

    try {
        iipc->Create(builder);
    } catch (std::exception e) {
        cout << e.what() << endl;
    }

    string container;

    auto conn = builder.GetTopology();

    conn->Recv(container, 18);

    LOG(DEBUG, "Server Recv:", container);

    conn->Send("Hello doori world.");

    /* 클라이언트 종료대기 */
    t.join();
}