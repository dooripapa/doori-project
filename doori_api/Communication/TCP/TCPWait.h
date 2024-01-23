//
// Created by jslee on 24. 1. 7.
//

#ifndef DOORI_PROJECT_TCPWAIT_H
#define DOORI_PROJECT_TCPWAIT_H

#include "TCPState.h"

namespace doori::api::Communication::TCP {

    class TCPNode;

    class TCPWait : public TCPState {
    public:
        void open(TCPNode &node) override;

        void establish(TCPNode &node) override;

        void close(TCPNode &node) override;

        void wait(TCPNode &node) override;

    } // doori

#endif //DOORI_PROJECT_TCPWAIT_H
