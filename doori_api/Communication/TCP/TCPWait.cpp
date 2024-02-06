//
// Created by jslee on 24. 1. 7.
//

#include "TCPWait.h"

namespace doori {
    namespace api {
        namespace Communication {
            namespace TCP {

                void TCPWait::establish(TCPNode *node) {
                    TCPState::establish(node);
                }

                void TCPWait::close(TCPNode *node) {
                    TCPState::close(node);
                }

                void TCPWait::wait(TCPNode *node) {
                    TCPState::wait(node);
                }
            } // TCP
        } // Communication
    } // api
} // doori
