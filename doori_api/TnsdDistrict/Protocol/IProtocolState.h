//
// Created by jaeseong on 23. 4. 28.
//
#ifndef DOORI_PROJECT_IPROTOCOLSTATE_H
#define DOORI_PROJECT_IPROTOCOLSTATE_H

#include "IProtocol.h"

namespace doori::TnsdDistrict::Protocol
{
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
        virtual void notify();
        virtual void listUp();
        virtual void changed();
        virtual void report();
        virtual void alive();
        virtual void pub();
        virtual void sub();
    protected:
        void changeState(IProtocol *protocol, IProtocolState *state);
    };
};

#endif //DOORI_PROJECT_IPROTOCOLSTATE_H
