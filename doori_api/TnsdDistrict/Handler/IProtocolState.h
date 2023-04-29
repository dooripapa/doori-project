//
// Created by jslee on 23. 4. 29.
//

#ifndef DOORI_PROJECT_IPROTOCOLSTATE_H
#define DOORI_PROJECT_IPROTOCOLSTATE_H

#include "TnsdDistrict/Protocol/Protocol.h"

namespace doori {
    namespace TnsdDistrict {
        namespace Protocol {

            /*
                구독자한테 notify
                자신을 등록하는 list up -> changed
                정보가 changed 구독자에게 notify
                자신을 상태를 report 함. tnsd와 정보가 다르면 changed
                자신이 alive 상태를 알림.
                자신을 pub으로 등록하면 list up 구독자한테 notify해서 알림
                자신을 sub으로 등록하면 list up
            */
            class IProtocolState {
            public:
                void notify();
                void listUp();
                void changed();
                void report();
                void alive();
                void pub();
                void sub();
            protected:
                void changeState(Protocol *protocol, IProtocolState *state);
            };

        } // doori
    } // TnsdDistrict
} // Handler

#endif //DOORI_PROJECT_IPROTOCOLSTATE_H
