//
// Created by jaeseong on 23. 1. 31.
//

#ifndef DOORI_PROJECT_ICONNECTION_H
#define DOORI_PROJECT_ICONNECTION_H

#include "DataStream/IStream.h"

namespace doori::CommunicationMember{
    class IConnection {
    public:
        virtual auto RequestFor(doori::DataStream::IStream data) -> bool = 0;
        virtual auto WaitFor(doori::DataStream::IStream& data) -> bool = 0;
        virtual auto Release() -> void = 0;
        virtual auto Init() -> void = 0;
    };
};



#endif //DOORI_PROJECT_ICONNECTION_H
