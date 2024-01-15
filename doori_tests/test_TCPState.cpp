//
// Created by jaeseong on 23. 5. 30.
//

#include <gtest/gtest.h>
#include <thread>
#include <functional>
#include "Common/Log.h"
#include "Communication/NodeModel.h"
#include "Communication/TCP/TCPNode.h"
#include "Communication/FromStrategies.h"
#include "Communication/ToStrategies.h"
#include "Communication/TCP/TCPEstablish.h"
#include "Communication/TCP/TCPOpen.h"

using namespace doori::api::Communication::TCP;
using namespace doori::api::Communication;

int receiver(const string &buffer) {

    PLOG(DEBUG, "recv Data[%t]", buffer);

    return 0;
}

int sender(const string &buffer, int bufferSize) {

    PLOG(DEBUG, "send Data[%t], size[%t]", buffer, bufferSize);

    return 0;
}


TEST(TCPState, Client) {
}

TEST(TCPState, Wait) {
    TCPNode tcpNode;

    tcpNode.tieSource("127.0.0.1", "8888"); //source binding IP Port

    NodeModel<TCPNode, FromStrategies, ToStrategies> nodeModel(tcpNode, FromStrategies{}, ToStrategies{});

    auto sock = nodeModel.From();

    tcpNode.setState(std::make_unique<ITCPState>(new TCPOpen()));
}

TEST(TCPState, Connect) {
    TCPNode tcpNode;

    tcpNode.tieSource("127.0.0.1", "8888"); //source binding IP Port
    tcpNode.tieRemote("127.0.0.1", "9999"); //destination IP Port

    NodeModel<TCPNode, FromStrategies, ToStrategies> nodeModel(tcpNode, FromStrategies{}, ToStrategies{});

    nodeModel.From();
    nodeModel.To();

    nodeModel.setState(new TCPEstablish());
}