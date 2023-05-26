//
// Created by jaeseong on 23. 1. 31.
//

#pragma once

#include <memory>
#include "CommunicationMember/Endpoint.h"
#include "IIPCTopology.h"

using namespace std;

namespace doori::CommunicationMember {

    enum TOPOLOGY_TYPE {
        SERVER,
        CLIENT
    };

    class IIPCBuilder {
    public:
        virtual unique_ptr<IIPCTopology> GetTopology() = 0;
        virtual int Establish() = 0;
    };

};
