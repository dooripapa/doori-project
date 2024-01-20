//
// Created by jslee on 24. 1. 6.
//

#ifndef DOORI_PROJECT_ITCPSTATE_H
#define DOORI_PROJECT_ITCPSTATE_H

namespace doori::api::Communication::TCP {
    class ITCPState {
    public:
        virtual void open() = 0;

        virtual void establish() = 0;

        virtual void close() = 0;

        virtual void wait() = 0;
    };
}


#endif //DOORI_PROJECT_ITCPSTATE_H
