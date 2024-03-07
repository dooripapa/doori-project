//
// Created by jslee on 24. 1. 7.
//

#ifndef DOORI_PROJECT_TCPCLOSE_H
#define DOORI_PROJECT_TCPCLOSE_H

#include "TCPState.h"
#include "Common/Log.h"
#include "Common/Error.h"

namespace doori::api::Communication::TCP {

    class TCPNode;

    class TCPClose : public TCPState, public Common::Error {
    public:
        void close(TCPNode *node) override;

        void establish(TCPNode *node) override;

        void listen(TCPNode *node) override;

        static TCPState *Instance();

    private:
        static TCPClose myInstance;
    };

} // doori

#endif //DOORI_PROJECT_TCPCLOSE_H
