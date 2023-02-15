//
// Created by jaeseong on 23. 2. 7.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include "TCPConnection.h"
#include "Common/Log.h"

namespace doori {
    namespace CommunicationMember {
        int TCPConnection::WaitFor(DataStream::IStream &sendStream) {

            char errorStr[1024]={0};
            auto ret = 0;
            auto nConnSock = 0;
            struct sockaddr_in sockaddrIncli;

            if(listen(mSourceBindFd, 1 ) < 0 )
            {
                LOG(ERROR, "TCP socket fd, fail to listen:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return -1;
            }

            socklen_t len = sizeof(struct sockaddr_in);
            if((ret = accept(mSourceBindFd, (struct sockaddr*)&sockaddrIncli, &len )) < 0 )
            {
                LOG(ERROR, "TCP socket fd, fail to accept:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return -1;
            }
            nConnSock = ret;
            return nConnSock;
        }

        TCPConnection::TCPConnection(int From, int To) : mSourceBindFd{From}, mDestBindFd{To} {

        }

        void TCPConnection::RequestTo(DataStream::IStream sendStream) {

        }

    } // doori
} // CommunicationMember