//
// Created by jaeseong on 23. 5. 8.
//

#ifndef DOORI_PROJECT_STATE_H
#define DOORI_PROJECT_STATE_H

#include "IProcedure.h"

namespace doori {
    namespace TnsDaemon {
        namespace Handler {

           class State {
            public:
                virtual void notify(IProcedure *procedure);
                virtual void listUp(IProcedure *procedure);
                virtual void changed(IProcedure *procedure);
                virtual void report(IProcedure *procedure);
                virtual void alive(IProcedure *procedure);
                virtual void pub(IProcedure *procedure);
                virtual void sub(IProcedure *procedure);
                virtual void send(IProcedure *procedure);
                virtual void recv(IProcedure *procedure);
                void changeState(IProcedure *procedure, State *state);
            };

        } // doori
    } // TnsdDistrcit
} // Handler

#endif //DOORI_PROJECT_STATE_H
