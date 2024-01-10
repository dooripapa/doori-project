//
// Created by jaeseong on 24. 1. 10.
//

#ifndef DOORI_PROJECT_TOSTRATEGIES_H
#define DOORI_PROJECT_TOSTRATEGIES_H

#include "TCP/TCPNode.h"

namespace doori::api::Communication {
    class ToStrategies {
    public:
        int operator()(TCP::TCPNode& node);

    };
}


#endif //DOORI_PROJECT_TOSTRATEGIES_H
