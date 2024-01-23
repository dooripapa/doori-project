//
// Created by jslee on 24. 1. 7.
//

#include "TCPClose.h"
#include "TCPNode.h"

namespace doori {
    namespace api {
        namespace Communication {
            namespace TCP {

                //명시적으로 사전 초기화.
                TCPClose TCPClose::myInstance;

                void TCPClose::close(TCPNode *node) {
                    LOG(INFO, "TCP close");
                }

                void TCPClose::establish(TCPNode *node) {
                    LOG(INFO, "TCP close(establish)");
                }

                void TCPClose::wait(TCPNode *node) {
                    LOG(INFO, "TCP close(wait)");
                }

                TCPState *TCPClose::Instance() {
                    return &myInstance;
                }
            } // TCP
        } // Communication
    } // api
} // doori