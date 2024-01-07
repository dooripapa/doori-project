//
// Created by jslee on 24. 1. 7.
//

#ifndef DOORI_PROJECT_TCPCONNECTION_H
#define DOORI_PROJECT_TCPCONNECTION_H

#include <memory>
#include "ITCPState.h"
#include "TCPTopologyNode.h"

namespace doori {
    namespace api {
        namespace Communication {
            namespace TCP {

                class TCPConnection {
                public:
                    TCPConnection(std::unique_ptr<TCPTopologyNode> node);

                public:
                    std::unique_ptr<ITCPState> state;
                    std::unique_ptr<TCPTopologyNode> node;
                };

            } // TCP
        } // Communication
    } // api
} // doori

#endif //DOORI_PROJECT_TCPCONNECTION_H
