//
// Created by jaeseong on 23. 1. 31.
//

#pragma once

#include <memory>
#include "IIPCTopology.h"

using namespace std;

namespace doori::Communication{

    class IIPCBuilder {
    public:
        virtual unique_ptr<IIPCTopology> GetTopology() = 0;
        virtual int Establish() = 0;
    };

};
