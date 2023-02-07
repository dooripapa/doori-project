//
// Created by jaeseong on 23. 2. 7.
//

#ifndef DOORI_PROJECT_ICONNECTION_H
#define DOORI_PROJECT_ICONNECTION_H

#include "DataStream/IStream.h"

namespace doori {
    namespace CommunicationMember {
        class IConnection {
        protected:
            virtual int WaitFor(DataStream::IStream &iStream) = 0;
            virtual int SendTo(DataStream::IStream iStream) = 0;
        };
    }
}

#endif //DOORI_PROJECT_ICONNECTION_H
