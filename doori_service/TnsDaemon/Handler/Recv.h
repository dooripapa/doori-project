//
// Created by jaeseong on 23. 5. 8.
//

#ifndef DOORI_PROJECT_RECV_H
#define DOORI_PROJECT_RECV_H

#include "IProcedure.h"
#include "State.h"

namespace doori {
    namespace TnsDaemon {
        namespace Handler {

            class Recv : public State {
            public:
                static State* getInstance();
                void recv(IProcedure *procedure) override;
            private:
                static State * mState;
            };

        } // doori
    } // TnsDaemon
} // Handler

#endif //DOORI_PROJECT_RECV_H
