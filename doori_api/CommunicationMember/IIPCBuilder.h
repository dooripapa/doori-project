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
        virtual unique_ptr<IIPCTopology> GetListenRsc() = 0;
        virtual bool EstablishTopologies() = 0;
    };

};
