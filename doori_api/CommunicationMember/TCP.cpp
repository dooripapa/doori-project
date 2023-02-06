//
// Created by jaeseong on 23. 1. 31.
//

#include <memory>
#include <cstring>
#include "TCP.h"
#include "IIPCBuilder.h"

namespace doori::CommunicationMember{

    TCP::~TCP() {

    }

    TCP::TCP() {

    }

    auto TCP::Create(IIPCBuilder &builder) -> unique_ptr<IConnection> {

    }
}

