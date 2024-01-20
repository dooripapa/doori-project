//
// Created by jslee on 24. 1. 7.
//

#include "TCPClose.h"

namespace doori {
    namespace api {
        namespace Communication {
            namespace TCP {
                void TCPClose::close() {
                    LOG(INFO, "TCP close");
                }

                void TCPClose::establish() {
                    LOG(INFO, "TCP close(establish)");
                }

                void TCPClose::open() {
                    LOG(INFO, "TCP close(open)");
                }

                void TCPClose::wait() {
                    LOG(INFO, "TCP close(wait)");
                }
            } // TCP
        } // Communication
    } // api
} // doori