//
// Created by jslee on 24. 1. 7.
//

#ifndef DOORI_PROJECT_TCPESTABLISH_H
#define DOORI_PROJECT_TCPESTABLISH_H

#include "TCPState.h"
#include "Common/Log.h"
#include "TCPNode.h"

namespace doori::api::Communication::TCP {

    class TCPEstablish : public TCPState {

    public:
        void establish(TCPNode *node) override;

        void close(TCPNode *node) override;

        void listen(TCPNode *node) override;
    };

} // doori

#endif //DOORI_PROJECT_TCPESTABLISH_H
