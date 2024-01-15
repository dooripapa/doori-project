//
// Created by jslee on 24. 1. 6.
//

#ifndef DOORI_PROJECT_ITCPSTATE_H
#define DOORI_PROJECT_ITCPSTATE_H

#include "TCPNode.h"

namespace doori::api::Communication::TCP {
    class ITCPState {
    public:
        virtual void open(TCPNode *node) = 0;
        virtual void establish(TCPNode *node)= 0;
        virtual void close(TCPNode *node)= 0;
        virtual void wait(TCPNode *node)= 0;
    };
}


#endif //DOORI_PROJECT_ITCPSTATE_H
