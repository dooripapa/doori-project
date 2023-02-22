// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Connection.h"

using namespace std;

namespace doori::CommunicationMember {

    int Connection::waitDataUtill(int iSocketfd, string &emptyBuff, ssize_t requestedLen) {
        /*
        * malloc은 주어진 크기도 보다 더 크게 잡는다
        */
        char errorStr[1024] = {0};
        char *pcContainer = (char *) malloc(requestedLen + 1);
        if (!pcContainer) {
            LOG(ERROR, "malloc fail");
            return -1;
        }
        memset(pcContainer, 0x00, sizeof(requestedLen + 1));
        char *pcPos = pcContainer;

        ssize_t readedLen = 0;
        size_t accumLen = requestedLen;

        while ((readedLen = read(iSocketfd, pcPos, accumLen)) < requestedLen) {
            if (readedLen == -1) {
                LOG(ERROR, "cant read data requested: ", strerror_r(errno, errorStr, sizeof(errorStr)));
                if (pcContainer)
                    free(pcContainer);
                return -1;
            }

            if (readedLen == 0) {
                LOG(INFO, "Socket FD : [", iSocketfd, "]", "closed");
                if (pcContainer)
                    free(pcContainer);
                return -2;
            }

            LOG(DEBUG, "pcPos:", pcPos);

            pcPos += readedLen;
            accumLen -= readedLen;
            if (!accumLen)
                break;
        }
        *(pcContainer + requestedLen) = 0x00;
        emptyBuff.assign(pcContainer);

        if (pcContainer)
            free(pcContainer);

        return 0;
    }

    auto Connection::connectTo(struct sockaddr_in destAddrIn, struct sockaddr_in sourceAddrIn) -> int {
        char errorStr[1024] = {0};
        int iRet = 0;
        int bindSock = -1;

        if (!mDest.CanRead()) {
            LOG(ERROR, "not defined Destination, need to() function ");
            return -1;
        }

        if (!mSource.CanRead()) {
            bindSock = socket(AF_INET, SOCK_STREAM, 0);
            if (bindSock < 0) {
                LOG(ERROR, "TCP socket fd, fail to open", strerror_r(errno, errorStr, sizeof(errorStr)));
                return -1;
            }
        } else {
            bindSock = Bind(sourceAddrIn);
            if( bindSock < 0 ) {
                LOG(ERROR, "bind error");
                return -1;
            }
        }

        iRet = connect(bindSock, (struct sockaddr *)&destAddrIn, sizeof(struct sockaddr_in));
        if (iRet < 0) {
            LOG(ERROR, "TCP socket fd, fail to connect:", strerror_r(errno, errorStr, sizeof(errorStr)));
            close(bindSock);
            return -1;
        }
        return bindSock;
    }

    auto Connection::send(int fd, const Stream &stream) -> int {
        auto ret = 0;
        ret = processSend(fd, stream);
        if (ret < 0)
            LOG(ERROR, "send fail : [", fd, "]");

        return ret;
    }

    int
    Connection::sendTo(int socketFd, const Stream &stream) {
        auto ret = 0;
        ret = processSend(socketFd, stream);
        if (ret < 0)
            LOG(ERROR, "sendTo fail : [", socketFd, "]");

        return ret;
    }

    auto Connection::onListening(struct sockaddr_in sockaddrIn) -> int {
        char errorStr[1024] = {0};

        auto fd = Bind(sockaddrIn);
        if (fd < 0) {
            LOG(ERROR, "onListening error");
            return -1;
        }

        if (listen(fd, 1) < 0) {
            LOG(ERROR, "TCP socket fd, fail to listen:", strerror_r(errno, errorStr, sizeof(errorStr)));
            return -1;
        }
        return fd;
    }

    int Connection::processBind(int &socketFd, const doori::CommunicationMember::Addr &addr) {
        char errorStr[1024] = {0};
        socketFd = socket(AF_INET, SOCK_STREAM, 0);
        if (socketFd < 0) {
            LOG(ERROR, "TCP socket fd, fail to open");
            return -1;
        }

        if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (void *) &(addr.getInetAddr()), sizeof(struct sockaddr_in)) <
            0) {
            LOG(ERROR, "TCP socket fd, fail to setsockopt");
            return -1;
        }

        if (bind(socketFd, (struct sockaddr *) &(addr.getInetAddr()), sizeof(struct sockaddr_in)) < 0) {
            LOG(ERROR, "TCP socket fd, fail to bind:", strerror_r(errno, errorStr, sizeof(errorStr)));
            return -1;
        }
        return 0;
    }

    int Connection::Bind(struct sockaddr_in &addrIn) {
        int fd = -1;
        char errorStr[1024] = {0};
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            LOG(ERROR, "TCP socket fd, fail to open");
            return -1;
        }

        if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, (void *) &addrIn, sizeof(struct sockaddr_in)) <
            0) {
            LOG(ERROR, "TCP socket fd, fail to setsockopt");
            return -1;
        }

        if (bind(fd, (struct sockaddr *) &addrIn, sizeof(struct sockaddr_in)) < 0) {
            LOG(ERROR, "TCP socket fd, fail to bind:", strerror_r(errno, errorStr, sizeof(errorStr)));
            return -1;
        }
        return fd;
    }

    auto Connection::onAccepting(int fd, struct sockaddr_in sockAddrIn) -> int {
        char errorStr[1024] = {0};
        int accpetFd= 0;
        socklen_t len = sizeof(struct sockaddr_in);
        if ((accpetFd = accept(fd, (struct sockaddr *) &sockAddrIn, &len)) < 0) {
            LOG(ERROR, "TCP socket fd, fail to accept:", strerror_r(errno, errorStr, sizeof(errorStr)));
            return -1;
        }
        return accpetFd;
    }

    auto Connection::waitFor(struct sockaddr_in sockaddrIn) -> int {
        char errorStr[1024] = {0};

        auto fd = Bind(sockaddrIn);
        if (fd < 0) {
            LOG(ERROR, "waitFor, binding error");
            return -1;
        }

        if (listen(fd, 1) < 0) {
            LOG(ERROR, "TCP socket fd, fail to listen:", strerror_r(errno, errorStr, sizeof(errorStr)));
            return -1;
        }

        socklen_t len = sizeof(struct sockaddr_in);
        auto acceptFd= 0;
        if ((acceptFd = accept(fd, (struct sockaddr *) &sockaddrIn, &len)) < 0) {
            LOG(ERROR, "TCP socket fd, fail to accept:", strerror_r(errno, errorStr, sizeof(errorStr)));
            return -1;
        }
        return acceptFd;
    }

    ///@see STX context start (0x02)\
    ETX context end (0x03)\
    STX LLLLLEEEEEEEE $|...doori_data...|$ ETX \
       123456789|123456789|123456789|
    auto Connection::recv(int fd, Stream &container) -> int {
        auto ret = 0;
        ret = processRecv(fd, container);
        if (ret < 0)
            LOG(ERROR, "recv");

        return ret;
    }

    int Connection::recvFrom(int recvSocket, Stream &container) {
        auto ret = 0;
        ret = processRecv(recvSocket, container);
        if (ret < 0)
            LOG(ERROR, "processRecv error :", ret);

        return ret;
    }

    auto
    Connection::reply(int fd, const Stream &contents, Stream &response_contents) -> int {
        auto ret = 0;
        ret = processSend(fd, contents);
        if (ret < 0) {
            LOG(ERROR, "fail to send");
            return -1;
        }
        ret = processRecv(fd, response_contents);
        if (ret < 0) {
            LOG(ERROR, "fail to recv");
            return -1;
        }
        return 0;
    }

/**
*@brief  소켓 send 처리하는 본체함수
*@details
*@date
*@param
*@return
	-3 : system error
	-2 : resource error
	-1 : user error
*@bug 
*@see
*/
    int
    Connection::processSend(int connected_socketFd, const Stream &contents) {
        char errorStr[1024] = {0};
        ssize_t len = 0;

        ssize_t requestLen = contents.toByteStream().length();
        if (requestLen == 0) {
            LOG(ERROR, "Contents length for Sending is zero");
            return -1;
        }
        ssize_t sendLen = requestLen;

        char *pcDataPos = (char *) malloc(contents.toByteStream().length() + 1);
        if (!pcDataPos) {
            LOG(FATAL, "malloc error");
            return -2;
        }

        memcpy(pcDataPos, contents.toByteStream().c_str(), requestLen);
        *(pcDataPos + requestLen) = 0x00;

        while ((len += write(connected_socketFd, pcDataPos, sendLen)) < requestLen) {
            if (len == -1) {
                LOG(ERROR, "send door_stream to Destination, fail to write()",
                    strerror_r(errno, errorStr, sizeof(errorStr)));
                if (pcDataPos)
                    free(pcDataPos);
                return -2;
            }
            pcDataPos += len;
            sendLen -= len;
            if (!sendLen)
                break;
        }
        if (pcDataPos)
            free(pcDataPos);
        return 0;
    }

    int Connection::Send(int fd, byte data[], ssize_t dataLen) {
        char errorStr[1024] = {0};
        ssize_t len = 0;

        ssize_t requestLen = dataLen;
        if (requestLen == 0) {
            LOG(ERROR, "Contents length for Sending is zero");
            return -1;
        }
        ssize_t sendLen = requestLen;

        char *pcDataPos = (char *) malloc(dataLen + 1);
        if (!pcDataPos) {
            LOG(FATAL, "malloc error");
            return -2;
        }

        memcpy(pcDataPos, data, requestLen);
        *(pcDataPos + requestLen) = 0x00;

        while ((len += write(fd, pcDataPos, sendLen)) < requestLen) {
            if (len == -1) {
                LOG(ERROR, "send door_stream to Destination, fail to write()",
                    strerror_r(errno, errorStr, sizeof(errorStr)));
                if (pcDataPos)
                    free(pcDataPos);
                return -2;
            }
            pcDataPos += len;
            sendLen -= len;
            if (!sendLen)
                break;
        }
        if (pcDataPos)
            free(pcDataPos);
        return 0;
    }

    ///@see STX context start (0x02) \
            ETX context end (0x03) \
            STX LLLLLEEEEEEEE $|...doori_data...|$ ETX \
            123456789|123456789|123456789|
    int Connection::processRecv(int connected_socketFd, Stream &recv_buffer) {
        int iRet = 0;
        //헤더의 길이를 읽는다
        //STX가 존재하지 않으면 세션 종료
        ssize_t bodyLen = 0;
        string lenStr;

        do {
            // 길이가 저장된 버퍼을 우선 저장
            if ((iRet = waitDataUtill(connected_socketFd, lenStr, 6)) < 0) {
                LOG(ERROR, "waitDataUtill error : [", iRet, "]");
                break;
            }
            // 앞에 코드값 확인
            if (lenStr.at(0) != 0x02) {
                LOG(ERROR, "STX not existing.");
                iRet = -1;
                break;
            }

            // stoi 사용할때 주의할점, 앞에 시작은 숫자문자열로 시작되어야 함
            string strTempLen(lenStr, 1, 6 - 1);
            string bodyStr;
            bodyLen = stoi(strTempLen);
            if ((iRet = waitDataUtill(connected_socketFd, bodyStr, bodyLen)) < 0) {
                LOG(ERROR, "waitDataUtill error : [", iRet, "]");
                break;
            }
            // 바디부분 마지막문자타입 체크, 정합성체크
            if (bodyStr.at(bodyLen - 1) != 0x03) {
                LOG(ERROR, "ETX not existing.");
                iRet = -1;
                break;
            }

            //body 부분에 인코딩값 가져오기
            //-1 : 0x03's length
            string encordingInfo(bodyStr, 0, 8);
            string dooriDataStr(bodyStr, 8, bodyLen - encordingInfo.length() - 1);
            if (recv_buffer.setString(encordingInfo, dooriDataStr) < 0) {
                LOG(ERROR, "setStream() fail.");
                return -1;
            }
        } while (false);

        if (iRet == -2)
            LOG(INFO, "Socket closed");

        return iRet;
    }

    auto Connection::setFrom(Endpoint source_endpoint) noexcept -> void {
        mSource = source_endpoint;
    }

    auto Connection::setTo(Endpoint dest_endpoint) noexcept -> void {
        mDest = dest_endpoint;
    }

    auto Connection::From() noexcept -> Endpoint & {
        return mSource;
    }

    auto Connection::To() noexcept -> Endpoint & {
        return mDest;
    }

}
