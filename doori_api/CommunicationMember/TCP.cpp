//
// Created by jaeseong on 23. 1. 31.
//

#include <memory>
#include <cstring>
#include "TCP.h"
#include "IIPCBuilder.h"
#include "IConnection.h"
#include "TCPConnection.h"

namespace doori::CommunicationMember{

    TCP::~TCP() {

    }

    TCP::TCP() {

    }

    auto TCP::Create(IIPCBuilder &builder) -> unique_ptr<IConnection> {

        auto fromFd = builder.InitFrom();
        auto toFd = builder.InitTo();

        auto r = make_unique<IConnection>(TCPConnection{fromFd, toFd});
        return r;
    }
}

