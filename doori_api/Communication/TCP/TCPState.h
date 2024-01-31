//
// Created by jslee on 24. 1. 6.
//

#ifndef DOORI_PROJECT_TCPSTATE_H
#define DOORI_PROJECT_TCPSTATE_H

#include "TCPNode.h"
#include "Common/Error.h"

namespace doori::api::Communication::TCP {

    class TCPNode;

    class TCPState : public Error{
    public:
        virtual void establish(TCPNode *node);

        virtual void close(TCPNode *node);

        virtual void wait(TCPNode *node);
    protected:
        static void changeState(TCPNode *node, TCPState *state);

    };
}


#endif //DOORI_PROJECT_TCPSTATE_H
