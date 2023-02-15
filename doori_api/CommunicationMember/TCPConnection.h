//
// Created by jaeseong on 23. 2. 7.
//

#ifndef DOORI_PROJECT_TCPCONNECTION_H
#define DOORI_PROJECT_TCPCONNECTION_H

#include "IConnection.h"

namespace doori {
    namespace CommunicationMember {

        class TCPConnection : IConnection {
        public:
            TCPConnection(int From, int To);

        private:
            void RequestTo(DataStream::IStream sendStream) override;

            void WaitFor(DataStream::IStream &sendStream) override;

        private:
            int mSourceBindFd;
            int mDestBindFd;
        };

    } // doori
} // CommunicationMember

#endif //DOORI_PROJECT_TCPCONNECTION_H
