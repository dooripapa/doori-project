//
// Created by jaeseong on 23. 1. 31.
//

#include "TCP.h"
#include "Common/Log.h"

namespace doori::api::Communication{

    void TCP::Create(IIPCBuilder &builder) {
        if( builder.Establish() != 0)
        {
            throw std::runtime_error("Establish");
        }
    }
}

