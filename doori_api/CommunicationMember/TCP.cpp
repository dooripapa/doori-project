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

    IConnection *TCP::Create(IIPCBuilder &builder) {
        return new TCPConnection(builder.getFrom(), builder.getTo());
    }

}

