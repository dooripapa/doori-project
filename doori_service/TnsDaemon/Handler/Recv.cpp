//
// Created by jaeseong on 23. 5. 8.
//

#include "Recv.h"

namespace doori {
    namespace TnsDaemon {
        namespace Handler {
            void Recv::recv(IProcedure *procedure) {

                // Stream 수신후
                switch (condition) {

                }

                changeState(procedure, )
            }

            State *Recv::getInstance() {
                if( mState != nullptr )
                    return mState;

                mState = new State();

                return mState;
            }
        } // doori
    } // TnsDaemon
} // Handler