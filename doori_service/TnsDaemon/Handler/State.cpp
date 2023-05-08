//
// Created by jaeseong on 23. 5. 8.
//

#include "State.h"

namespace doori {
    namespace TnsDaemon {
        namespace Handler {

            void State::changeState(IProcedure *procedure, State *state) {
                procedure->changedState(state);
            }

            void State::notify(IProcedure *procedure) {

            }

            void State::listUp(IProcedure *procedure) {

            }

            void State::changed(IProcedure *procedure) {

            }

            void State::report(IProcedure *procedure) {

            }

            void State::alive(IProcedure *procedure) {

            }

            void State::pub(IProcedure *procedure) {

            }

            void State::sub(IProcedure *procedure) {

            }

            void State::send(IProcedure *procedure) {

            }

            void State::recv(IProcedure *procedure) {

            }
        } // doori
    } // TnsdDistrcit
} // Handler