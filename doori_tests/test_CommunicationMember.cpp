//
// Created by jaeseong on 23. 2. 2.
//
#include <gtest/gtest.h>
#include <thread>
#include "CommunicationMember/Endpoint.h"
#include "CommunicationMember/TCPBuilder.h"
#include "Common/Log.h"
#include "DataStream/Json.h"
#include "CommunicationMember/TCP.h"
#include "DataStream/Json.h"

using namespace doori::CommunicationMember;
using namespace doori::DataStream;
using namespace doori::Common;
using namespace std;

void RunClient() {

    std::unique_ptr<IIPC> iipc = std::make_unique<TCP>();

    std::unique_ptr<IIPCBuilder> builder = std::make_unique<TCPBuilder>(TOPOLOGY_TYPE::CLIENT, "127.0.0.1", "8888");

    try {
        sleep(2);

        iipc->Create( std::move(builder) );
    }catch (std::exception e) {
        cout << e.what() << endl;
    }

    string container;

    auto conn = iipc->GetIPC();

    conn->Send("Hello doori world.");

    conn->Recv(container, 18);

    LOG(DEBUG, "Client Recv:", container);
}

TEST(CommunicationMember, Usage)
{
    std::unique_ptr<IIPC> iipc = std::make_unique<TCP>();

    Json bindAddress, destAddress;

    bindAddress["ip"] = "127.0.0.1";
    bindAddress["port"] = "9999";

    destAddress["ip"] = "127.0.0.1";
    destAddress["port"] = "8888";

    Endpoint From{Endpoint::TYPE::TCP, bindAddress};
    Endpoint To{Endpoint::TYPE::TCP, destAddress};

    std::unique_ptr<IIPCBuilder> builder = std::make_unique<TCPBuilder>(TOPOLOGY_TYPE::SERVER, "", "8888");

    /* 클라이언트 접속 프로그램 기동,
     * 단, 2초뒤에 기동됨.기동하자마자,
     * 바로, 접속시도를 막기위해서*/
    std::thread t(RunClient);

    try {
        iipc->Create( std::move(builder) );
    }catch (std::exception e) {
        cout << e.what() << endl;
    }

    string container;

    auto conn = iipc->GetIPC();

    conn->Recv(container, 18);

    LOG(DEBUG, "Server Recv:", container);

    conn->Send("Hello doori world.");

    /* 클라이언트 종료대기 */
    t.join();
}
