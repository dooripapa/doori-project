//
// Created by jaeseong on 23. 2. 2.
//
#include <gtest/gtest.h>
#include "CommunicationMember/Connection.h"
#include "CommunicationMember/Endpoint.h"
#include "CommunicationMember/TCPBuilder.h"
#include "DataStream/Json.h"
#include "CommunicationMember/TCP.h"
#include "DataStream/IStream.h"
#include "DataStream/Json.h"

using namespace doori::CommunicationMember;
using namespace doori::DataStream;
using namespace std;

TEST(CommunicationMember, Usage)
{
    IIPC *iipc = new TCP();

    Json bindAddress, destAddress;

    bindAddress["ip"] = "127.0.0.1";
    bindAddress["port"] = "9999";

    destAddress["ip"] = "127.0.0.1";
    destAddress["port"] = "8888";

    Endpoint From{Endpoint::TYPE::TCP, bindAddress};
    Endpoint To{Endpoint::TYPE::TCP, destAddress};

//    TCPBuilder builder{From, To};

    auto tcpBuilder = new TCPBuilder(From, To);

    auto iConnection= iipc->Create( tcpBuilder );

    auto requestFd = iConnection->ConnectTo();

    doori::LOG(INFO, "CONNECT FD:[", requestFd, "]");

    auto listenFd = iConnection->WaitFor();

    doori::LOG(INFO, "Listen FD:[", listenFd, "]");
}