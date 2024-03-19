//
// Created by jaeseong on 23. 2. 2.
//
#include <gtest/gtest.h>
#include <thread>
#include "Communication/TCPBuilder.h"
#include "Common/Log.h"
#include "Communication/TCP.h"

using namespace doori::api::Communication;
using namespace doori::api::Common;
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

TEST(CommunicationMember, new_tcp_receiver){

    IPC ipc = TCP();
//    IPC ipc = SharedMemory();
//    IPC ipc = UDP();
//    IPC ipc = PIPE();
//    IPC ipc = MessageQueue();

    // 그 원복 객체 리턴 하는 패턴.
    auto node = ipc.getTopologyNode();

    node.Ip("192.168.1.1");
    node.Port("9999");

    node.setsockopt();
    node.setsockopt();
    node.setsockopt();
    node.setsockopt();

    auto fd = FromStrategy(node);
    auto accptFd = fd.waitFor();

    string container{};
    auto ret = 0;
    auto timeout = 10;
    while( ret = RecvStategy(accptFd, container, timeout) )
    {
        if(ret>0)
        {
            //received
        }
        else if (ret = 0)
        {
            //socket is closed
        }
        else{
            //timeout
        }
    }

    to.Ip("192.168.1.1");
    to.Port("9999");
    auto connFd = ToStrategy(to);
}

TEST(CommunicationMember, new_tcp_sender){

    IPC ipc = TCP();

    // 그 원복 객체 리턴 하는 패턴.
    auto fromNode = ipc.getTopologyNode();
    auto toNode = ipc.getTopologyNode();

    fromNode.Ip("192.168.1.1");
    fromNode.Port("9999");

    fromNode.setsockopt();

    auto fd = FromStrategy(fromNode);

    toNode.Ip("192.168.1.1");
    toNode.Port("9999");
    auto connFd = ToStrategy(toNode);

    auto ret = 0;
    string container{};
    container="leejaeseong";
    ret = SendStrategy(connFd, container );

}