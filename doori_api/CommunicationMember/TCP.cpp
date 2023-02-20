//
// Created by jaeseong on 23. 1. 31.
//

#include "TCP.h"

namespace doori::CommunicationMember{

    TCP::~TCP() {

    }

    TCP::TCP() {

    }

    IConnection *TCP::Create(IIPCBuilder *builder) {
        auto left = builder->getFrom();
        auto right = builder->getTo();
        return new TCPConnection(left, right);
    }

}

