//
// Created by jslee on 24. 1. 7.
//

#ifndef DOORI_PROJECT_TCPESTABLISH_H
#define DOORI_PROJECT_TCPESTABLISH_H

#include "TCPState.h"
#include "TCPNode.h"

namespace doori {
    namespace api {
        namespace Communication {
            namespace TCP {

                class TCPEstablish : public TCPState{

                public:
                    void wait(TCPNode *node) override;
                    void establish(TCPNode *node) override;
                    void close(TCPNode *node) override;
                };

            } // TCP
        } // Communication
    } // api
} // doori

#endif //DOORI_PROJECT_TCPESTABLISH_H
