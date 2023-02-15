//
// Created by jaeseong on 23. 2. 7.
//

#ifndef DOORI_PROJECT_ICONNECTION_H
#define DOORI_PROJECT_ICONNECTION_H

#include "DataStream/IStream.h"

namespace doori {
    namespace CommunicationMember {
        class IConnection {
            virtual void RequestTo(DataStream::IStream sendStream) = 0;
            virtual void WaitFor(DataStream::IStream& sendStream) = 0;
        };
    }
}

#endif //DOORI_PROJECT_ICONNECTION_H
