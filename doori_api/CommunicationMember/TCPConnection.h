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
            int ConnectTo() override;

            int WaitFor() override;

            int Send(int rscFd, const DataStream::IStream &data) override;

            int Recv(int rscFd, DataStream::IStream &data) override;

        private:
            int mSourceBindFd;
            int mDestBindFd;
        };

    } // doori
} // CommunicationMember

#endif //DOORI_PROJECT_TCPCONNECTION_H
