//
// Created by jaeseong on 23. 2. 7.
//

#ifndef DOORI_PROJECT_ICONNECTION_H
#define DOORI_PROJECT_ICONNECTION_H

#include "DataStream/IStream.h"

namespace doori {
    namespace CommunicationMember {
        class IConnection {
            virtual int ConnectTo() = 0;
            virtual int WaitFor() = 0;
            virtual int Send(int rscFd, const DataStream::IStream& data) = 0;
            virtual int Recv(int rscFd, DataStream::IStream& data) = 0;
        };
    }
}

#endif //DOORI_PROJECT_ICONNECTION_H
