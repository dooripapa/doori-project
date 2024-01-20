//
// Created by jslee on 24. 1. 7.
//

#ifndef DOORI_PROJECT_TCPESTABLISH_H
#define DOORI_PROJECT_TCPESTABLISH_H

#include "ITCPState.h"
#include "Log.h"

namespace doori {
    namespace api {
        namespace Communication {
            namespace TCP {

                class TCPEstablish : public ITCPState{

                public:
                    void wait() override;
                    void open() override;
                    void establish() override;
                    void close() override;
                };

            } // TCP
        } // Communication
    } // api
} // doori

#endif //DOORI_PROJECT_TCPESTABLISH_H
