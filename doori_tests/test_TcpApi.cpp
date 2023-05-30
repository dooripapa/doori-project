//
// Created by jaeseong on 23. 5. 30.
//

#include <gtest/gtest.h>
#include <thread>
#include "CommunicationMember/TCPBuilder.h"
#include "Common/Log.h"
#include "CommunicationMember/TcpApi.h"
#include "CommunicationMember/Socket.h"

using namespace doori::CommunicationMember;

TEST(TcpApi, Usage) {

    Socket socket{};

    TcpApi tcpApi{socket};

}
