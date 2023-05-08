//
// Created by jaeseong on 23. 5. 8.
//

#ifndef DOORI_PROJECT_NOTIFY_H
#define DOORI_PROJECT_NOTIFY_H

#include "State.h"

namespace doori {
    namespace TnsDaemon {
        namespace Handler {

            class Notify : public State {

            public:
                Notify() = delete;

                static State * getInstance();

            private:
                static State *mState;
            };

        } // doori
    } // TnsDaemon
} // Handler

#endif //DOORI_PROJECT_NOTIFY_H
