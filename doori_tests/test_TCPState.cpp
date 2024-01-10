//
// Created by jaeseong on 23. 5. 30.
//

#include <gtest/gtest.h>
#include <thread>
#include <functional>
#include "Common/Log.h"
#include "Communication/INode.h"
#include "Communication/TCP/TCPTopologyNode.h"
#include "Communication/TCP/TCPConnection.h"

using namespace doori::api::Communication::TCP;
using namespace doori::api::Communication;


TEST(TCPState, Client)
{
    TCPTopologyNode tcp;

    tcp.setState(new TCPinit("127.0.0.1", "8888"));
}

TEST(TCPState, Remote)
{
    auto node = std::make_unique<INode>();

    node.from("127.0.0.1", "9999");
    node.to("127.0.0.1", "8888");
    node.setHandler();

    std::function<int(ITopologyNode const &node)> processing;
    processing = [](ITopologyNode const &node) {

        std::string buffer;
        std::int32_t bufferSize = 200;

        node.Recv(buffer, bufferSize);

        PLOG(DEBUG, "Recv Data[%t]", buffer);

        return 0;
    };

    TCPConnection conn{node};

    conn.setState(new TCPwait(processing));
}