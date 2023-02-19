//
// Created by jaeseong on 23. 2. 2.
//
#include <gtest/gtest.h>
#include "CommunicationMember/Addr.h"
#include "CommunicationMember/Connection.h"
#include "CommunicationMember/Endpoint.h"
#include "CommunicationMember/TCPBuilder.h"
#include "DataStream/Json.h"
#include "CommunicationMember/TCP.h"
#include "CommunicationMember/IIPC.h"
#include "DataStream/IStream.h"
#include "DataStream/Json.h"
#include <iostream>

using namespace doori::CommunicationMember;
using namespace doori::DataStream;
using namespace std;

TEST(CommunicationMember, Usage)
{
    TCP tcp{};
    IIPC *iipc = &tcp;

    Json bindAddress, destAddress;

    bindAddress["ip"] = "127.0.0.1";
    bindAddress["port"] = "8888";

    destAddress["ip"] = "127.0.0.1";
    destAddress["port"] = "8888";

    Endpoint From{Endpoint::TYPE::TCP, bindAddress};
    Endpoint To{Endpoint::TYPE::TCP, destAddress};

    TCPBuilder builder{From, To};

    auto iConnection= iipc->Create( builder );

    auto requestFd = iConnection->ConnectTo();

    auto listenFd = iConnection->WaitFor();

}
