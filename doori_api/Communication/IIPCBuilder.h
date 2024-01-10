//
// Created by jaeseong on 23. 1. 31.
//

#pragma once

#include <memory>
#include "ITopologyNode.h"

using namespace std;

namespace doori::api::Communication{

    class IIPCBuilder {
    public:
        virtual unique_ptr<ITopologyNode> GetTopology() = 0;
        virtual int Establish() = 0;
    };

};
