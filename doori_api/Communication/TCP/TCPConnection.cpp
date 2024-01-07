//
// Created by jslee on 24. 1. 7.
//

#include "TCPConnection.h"

namespace doori {
    namespace api {
        namespace Communication {
            namespace TCP {

                TCPConnection::TCPConnection(std::unique_ptr<TCPTopologyNode> node)
                        : state(new ITCPState()), node(std::move(node)) {

                }
            } // TCP
        } // Communication
    } // api
} // doori