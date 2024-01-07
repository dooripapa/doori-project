//
// Created by jaeseong on 23. 5. 30.
//

#include <gtest/gtest.h>
#include <thread>
#include <functional>
#include "Common/Log.h"
#include "Communication/IIPCTopologyNode.h"
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
    auto node = std::make_unique<TCPTopologyNode>();
    node.bind("127.0.0.1", "9999");

    TCPConnection conn{node};

    std::function<int(IIPCTopologyNode const &node)> processing;

    processing = [](IIPCTopologyNode const &node) {

        std::string buffer;
        std::int32_t bufferSize = 200;

        node.Recv(buffer, bufferSize);

        PLOG(DEBUG, "Recv Data[%t]", buffer);

        return 0;
    };

    tcp.setState(new TCPwait("127.0.0.1", "8888", processing));

}