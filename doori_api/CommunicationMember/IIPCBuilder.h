//
// Created by jaeseong on 23. 1. 31.
//

#pragma once

#include <memory>
#include "IConnection.h"

using namespace std;

namespace doori::CommunicationMember {

    class IIPCBuilder {
    public:
        enum class Type {TCP, UDP, SHM, MSQ};
        virtual Type GetTopologies() = 0;
        virtual unique_ptr<IConnection> GetConnection() = 0;
        virtual void InitFrom() = 0;
        virtual void InitTo() = 0;
    };

};
