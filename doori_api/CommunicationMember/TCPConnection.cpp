//
// Created by jaeseong on 23. 2. 7.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include "TCPConnection.h"
#include "Common/Log.h"

namespace doori {
    namespace CommunicationMember {
        int TCPConnection::WaitFor() {

            char errorStr[1024] = {0};
            auto ret = 0;
            auto nConnSock = 0;
            struct sockaddr_in sockaddrIncli;

            if (listen(mSourceBindFd, 1) < 0) {
                LOG(ERROR, "TCP socket fd, fail to listen:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return -1;
            }

            socklen_t len = sizeof(struct sockaddr_in);
            if ((ret = accept(mSourceBindFd, (struct sockaddr *) &sockaddrIncli, &len)) < 0) {
                LOG(ERROR, "TCP socket fd, fail to accept:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return -1;
            }
            nConnSock = ret;
            return nConnSock;
        }

        TCPConnection::TCPConnection(int From, int To) : mSourceBindFd{From}, mDestBindFd{To} {

        }

        int TCPConnection::ConnectTo() {
            int iRet = 0;
            int nSocketFd = 0;
            char errorStr[1024] = {0};
            nSocketFd = socket(AF_INET, SOCK_STREAM, 0);

            iRet = connect(mDestBindFd, (struct sockaddr *) &(addr.getInetAddr()), sizeof(struct sockaddr_in));
            if (iRet < 0) {
                LOG(ERROR, "TCP socket fd, fail to connect:", strerror_r(errno, errorStr, sizeof(errorStr)));
                close(mBindSock);
                mBindSock = -1;
                return -1;
            }
            mConnSock = mBindSock;
            return mConnSock;
        }

        int TCPConnection::Send(int rscFd, const DataStream::IStream &data) {
            return 0;
        }

        int TCPConnection::Recv(int rscFd, DataStream::IStream &data) {
            return 0;
        }

    } // doori
} // CommunicationMember