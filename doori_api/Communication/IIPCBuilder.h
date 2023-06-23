//
// Created by jaeseong on 23. 1. 31.
//

#pragma once

#include <memory>
#include "IIPCTopologyNode.h"

using namespace std;

namespace doori::api::Communication{

    class IIPCBuilder {
    public:
        virtual unique_ptr<IIPCTopologyNode> GetTopology() = 0;
        virtual int Establish() = 0;
    };

};
