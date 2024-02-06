//
// Created by jslee on 24. 1. 7.
//

#include "TCPEstablish.h"

namespace doori {
    namespace api {
        namespace Communication {
            namespace TCP {

                void TCPEstablish::wait(TCPNode *node) {
                    TCPState::wait(node);
                }

                void TCPEstablish::establish(TCPNode *node) {
                    TCPState::establish(node);
                }

                void TCPEstablish::close(TCPNode *node) {
                    TCPState::close(node);
                }
            } // TCP
        } // Communication
    } // api
} // doori