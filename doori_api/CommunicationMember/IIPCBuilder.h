//
// Created by jaeseong on 23. 1. 31.
//

#pragma once

#include <memory>

using namespace std;

namespace doori::CommunicationMember {

    class IIPCBuilder {
    public:
        virtual int InitFrom() = 0;
        virtual int InitTo() = 0;
    };

};
