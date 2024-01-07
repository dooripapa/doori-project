//
// Created by jslee on 24. 1. 7.
//

#ifndef DOORI_PROJECT_TCPCLOSE_H
#define DOORI_PROJECT_TCPCLOSE_H

#include "ITCPState.h"

namespace doori {
    namespace api {
        namespace Communication {
            namespace TCP {

                class TCPClose : public ITCPState{
                public:
                    void close() override;
                    void establish() override;
                    void open() override;
                    void wait() override;
                };

            } // TCP
        } // Communication
    } // api
} // doori

#endif //DOORI_PROJECT_TCPCLOSE_H
