//
// Created by jaeseong on 23. 2. 2.
//
#include <gtest/gtest.h>
#include "CommunicationMember/Addr.h"
#include "CommunicationMember/Connection.h"
#include "CommunicationMember/Endpoint.h"
#include "CommunicationMember/Epoll.h"
#include "CommunicationMember/EpollEvents.h"
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
    unique_ptr<IIPC> tcp= make_unique<TCP>();

    Json bindAddress, destAddress;

    bindAddress["ip"] = "127.0.0.1";
    bindAddress["port"] = "8888";

    destAddress["ip"] = "127.0.0.1";
    destAddress["port"] = "8888";

    Endpoint From{Endpoint::TYPE::TCP, bindAddress};
    Endpoint To{Endpoint::TYPE::TCP, destAddress};

    TCPBuilder builder{From, To};

    auto Connection = tcp.Create( builder );

    Connection.RequestTo(IStream sendStream);

    Connection.WaitFor(IStream& recvStream);

}
