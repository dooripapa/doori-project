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
        virtual Type GetType() = 0;
        virtual unique_ptr<IConnection> GetConnection() = 0;
    protected:
        Endpoint mFrom;
        Endpoint mTo;
    };

};
