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

    string TCP::GetOS() {
        const string OS{"linux"};
        return OS;
    }

    TCP::TCP() {

    }

    auto TCP::Create(IIPCBuilder &builder) -> unique_ptr<IConnection> {
        builder.Init(mIp)
        return unique_ptr<IConnection>();
    }
}

