//
// Created by jaeseong on 23. 1. 31.
//

#pragma once

#include <memory>
#include "CommunicationMember/Endpoint.h"

using namespace std;

namespace doori::CommunicationMember {

    class IIPCBuilder {
    public:
        virtual auto getFrom() -> const Endpoint& = 0;
        virtual auto getTo() -> const Endpoint& = 0;
    };

};
