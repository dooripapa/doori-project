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

    const IConnection &TCP::Create(IIPCBuilder &builder) {
        TCPConnection tcpConnection{builder.getFrom(), builder.getTo()};
        return TCPConnection;
    }
}

