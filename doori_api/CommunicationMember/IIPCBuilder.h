//
// Created by jaeseong on 23. 1. 31.
//

#pragma once

#include <memory>
#include "CommunicationMember/Endpoint.h"

using namespace std;

namespace doori::CommunicationMember {

    enum TOPOLOGY_TYPE {
        SERVER,
        CLIENT
    };

    class IIPCBuilder {
    public:
        virtual int BindFrom() = 0;
        virtual int BindTo() = 0;
        virtual int GetListenRsc() = 0;
        virtual int EstablishTopologies() = 0;
    };

};
