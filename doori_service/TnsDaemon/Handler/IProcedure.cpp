//
// Created by jaeseong on 23. 5. 8.
//

#include "IProcedure.h"

namespace doori {
    namespace TnsDaemon {
        namespace Handler {

            IProcedure::IProcedure() {
                mState = Recv::getInstance();
            }

            void IProcedure::notify() {
                mState->notify(this);
            }

            void IProcedure::listUp() {
                mState->listUp(this);
            }

            void IProcedure::changed() {
                mState->changed(this);
            }

            void IProcedure::report() {
                mState->report(this);
            }

            void IProcedure::alive() {
                mState->alive(this);
            }

            void IProcedure::pub() {
                mState->pub(this);
            }

            void IProcedure::sub() {
                mState->sub(this);
            }

            void IProcedure::send() {
                mState->send(this);
            }

            void IProcedure::recv() {
                mState->recv(this);
            }

            void IProcedure::changedState(State * state) {
                mState = state;
            }

        } // doori
    } // TnsDaemon
} // Handler