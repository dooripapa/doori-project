//
// Created by jslee on 24. 1. 7.
//

#ifndef DOORI_PROJECT_TCPLISTEN_H
#define DOORI_PROJECT_TCPLISTEN_H

#include "TCPState.h"

namespace doori::api::Communication::TCP {

    class TCPNode;

    class TCPListen : public TCPState {
    public:
        void establish(TCPNode *node) override;

        void close(TCPNode *node) override;

        void listen(TCPNode *node) override;
    }; // doori
}
#endif //DOORI_PROJECT_TCPLISTEN_H