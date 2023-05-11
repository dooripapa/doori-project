//
// Created by jaeseong on 23. 3. 1.
//

#include <fcntl.h>
#include <functional>
#include "TcpApi.h"

namespace doori {
    namespace CommunicationMember {
        int TcpApi::Socket() {
            char errorStr[1024] = {0};
            auto sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                LOG(ERROR, "TCP socket fd, fail to open", strerror_r(errno, errorStr, sizeof(errorStr)));
                return -1;
            }
            return sock;
        }

        int TcpApi::SetReuseOpt(int fd, std::string ip, std::string port) {

            ::sockaddr_in sockaddrIn;
            memset(&sockaddrIn, 0x00, sizeof(::sockaddr_in));

            uint16_t u16Temp = 0;
            u16Temp = atoi(port.c_str());

            sockaddrIn.sin_family = AF_INET;
            sockaddrIn.sin_port = htons(u16Temp);
            sockaddrIn.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());

            if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, (void *) &sockaddrIn, sizeof(::sockaddr_in)) <
                0) {
                LOG(ERROR, "TCP socket fd, fail to setsockopt");
                return -1;
            }
            return fd;
        }

        int TcpApi::Bind(int fd, std::string ip, std::string port) {
            char errorStr[1024] = {0};
            ::sockaddr_in sockaddrIn;
            memset(&sockaddrIn, 0x00, sizeof(::sockaddr_in));

            uint16_t u16Temp = 0;
            u16Temp = atoi(port.c_str());

            sockaddrIn.sin_family = AF_INET;
            sockaddrIn.sin_port = htons(u16Temp);
            sockaddrIn.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());

            if (bind(fd, (struct sockaddr *) &sockaddrIn, sizeof(::sockaddr_in)) < 0) {
                LOG(ERROR, "TCP socket fd, fail to bind:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return -1;
            }
            return fd;
        }

        int TcpApi::Listen(int fd, int backlogNum) {
            char errorStr[1024] = {0};
            if (listen(fd, backlogNum) < 0) {
                LOG(ERROR, "TCP socket fd, fail to listen:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return -1;
            }
            return fd;
        }

        int TcpApi::Accept(int fd) {
            int acptFd = 0;
            char errorStr[1024] = {0};

            ::sockaddr_in sockaddrIn;

            socklen_t len = sizeof(::sockaddr_in);
            // sockaddrIn 변수는 접속한 클라이언트 주소 저장용도로 사용가능하다.
            if ((acptFd = accept(fd, (struct sockaddr *) &sockaddrIn, &len)) < 0) {
                LOG(ERROR, "TCP socket fd, fail to accept:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return -1;
            }
            return acptFd;
        }

        int TcpApi::Connect(int fd, string ip, string port) {
            char errorStr[1024] = {0};

            if (ip.empty() || port.empty()) {
                LOG(FATAL, "ip, port value is empty.");
                std::exit(-1);
            }

            ::sockaddr_in sockaddrIn;
            memset(&sockaddrIn, 0x00, sizeof(::sockaddr_in));

            uint16_t u16Temp = 0;
            u16Temp = atoi(port.c_str());

            sockaddrIn.sin_family = AF_INET;
            sockaddrIn.sin_port = htons(u16Temp);
            sockaddrIn.sin_addr.s_addr = inet_addr(ip.c_str());

            errno = 0;
            auto iRet = connect(fd, (struct sockaddr *)&sockaddrIn, sizeof(struct sockaddr_in));
            if (iRet < 0) {
                LOG(ERROR, "TCP socket fd, fail to connect:", strerror_r(errno, errorStr, sizeof(errorStr)));
                close(fd);
                return -1;
            }
            return fd;
        }

        int TcpApi::Send(int fd, const char *data, uint8_t dataLen) {
            char errorStr[1024] = {0};

            int nRet = 0;
            errno = 0;
            nRet = send(fd, data, dataLen, 0);
            if(nRet == -1)
            {
                LOG(ERROR, "TCP send(), fail to connect:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return -1;
            }
            LOG(DEBUG, "complete to send data! size[", nRet, "]");
            return 0;
        }

        int TcpApi::Recv(int fd, char *dataContainer, std::uint8_t dataLen) {
            char errorStr[1024] = {0};

            int nRet = 0;
            errno = 0;
            nRet = recv(fd, dataContainer, dataLen, MSG_WAITALL);
            if(nRet == -1)
            {
                LOG(ERROR, "TCP recv(), fail to connect:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return -1;
            }
            LOG(DEBUG, dataContainer);
            LOG(DEBUG, "recv data! size[", nRet, "]");

            return 0;
        }

        int TcpApi::SetTimeoutOpt(int fd, std::uint8_t timeout) {
            /*
              소켓 설정을 논블로킹으로 셋팅함
              그래야, recv()함수에서 Timeout를 설정할 수 있음
            */
            int flags = fcntl(fd, F_GETFL, 0);  // 소켓의 플래그 값을 가져옴

            fcntl(fd, F_SETFL, flags | O_NONBLOCK);  // 소켓의 플래그 값을 변경하여 넌블로킹 모드로 설정

            struct timeval tv;
            tv.tv_sec  = timeout;
            tv.tv_usec = 0;

            if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (void *) &tv, sizeof(struct timeval)) < 0) {
                LOG(ERROR, "TCP socket fd, fail to setsockopt");
                return -1;
            }
            return 0;
        }

        int TcpApi::RequestConnection(string ip, string port, std::uint8_t timeout) {
            char errorStr[1024] = {0};

            int fd = Socket();
            if (fd == -1)
            {
                LOG(ERROR, "RequestConnection error, at Socket()");
                return -1;
            }

            if(SetTimeoutOpt(fd, timeout) == -1)
            {
                LOG(ERROR, "RequestConnection error, at SetTimeoutOpt()");
                return -1;
            }

            if(Connect(fd, ip, port) == -1)
            {
                LOG(ERROR, "RequestConnection error, at Connect()");
                return -1;
            }
            return fd;
        }

        int TcpApi::Reply(int socketFd, const char *data, uint8_t dataLen, char *recvData, uint8_t tilDataSize) {
            int iRcvDataLen = 0;

            if( Send(socketFd, data, dataLen)== -1)
            {
                LOG(ERROR, "Reply error, at Send()");
                return -1;
            }

            iRcvDataLen =Recv(socketFd, recvData, tilDataSize);
            if( iRcvDataLen == -1)
            {
                LOG(ERROR, "Reply error, at Send()");
                return -1;
            }

            return iRcvDataLen;
        }

        int TcpApi::CreateEpoll(int socketFd, int backlogNum, std::function<int(int)> delegation ) {

            int iEpollFd = -1;
            int iListenSocket = -1;

            iEpollFd = epoll_create1(0);
            if( iEpollFd == -1 )
            {
                LOG(ERROR, "Create error" );
                return -1;
            }

            iListenSocket = doori::CommunicationMember::TcpApi::Listen(socketFd, backlogNum);
            if (iListenSocket == -1)
            {
                LOG(ERROR, "fail to make listening socket" );
                return -1;
            }

            //람다함수를 함수포인터로 변경함.
            //이 람다함수로 소켓을 등록하는 함수를 대신함.
            auto lambda = [=](){

                struct epoll_event ev{};
                int conn_sock;
                conn_sock = CommunicationMember::TcpApi::Accept(iListenSocket);
                LOG(INFO, "Accepted FD[", conn_sock,"]");
                if (conn_sock == -1) {
                    LOG(ERROR, "fail to accept socket.");
                    return -1;
                }

                // Timeout 설정함
                conn_sock = CommunicationMember::TcpApi::SetTimeoutOpt(conn_sock, 10);

                ev.events = EPOLLIN;
                ev.data.fd = conn_sock;
                auto function = reinterpret_cast< int(*)(int) >( delegation.target< int(int) >() );
                ev.data.ptr = (void*)function;
                if (epoll_ctl(iEpollFd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
                {
                    LOG(ERROR, "epoll, fail to EPOLL_CTL_ADD");
                    return -1;
                }
                return conn_sock;
            }; // 람다 끝

            std::function< int() > funcWapper = lambda;

            //Listen소켓으로 연결요청이 오면, 소켓을 epoll에 등록하는 함수를 호출하도록 사전에 저장
            void *pRegisterFunc = reinterpret_cast< void* >( funcWapper.target< int() >() );

            struct epoll_event ev{};
            memset(&ev, 0x00, sizeof(struct epoll_event) );

            ev.events = EPOLLIN;
            ev.data.ptr = pRegisterFunc;
            ev.data.fd = iListenSocket;
            if ( epoll_ctl(iEpollFd, EPOLL_CTL_ADD, iListenSocket, &ev) == -1)
            {
                LOG(ERROR, "fail to register listening socket to epoll list" );
                return -1;
            }

            return 0;
        }

        auto TcpApi::RunningEpoll(int epollFd, int listenSocket, int backlogEventNum, int timeout) -> void {

            char	errorStr[1024]={0};

            LOG(INFO, "==================================");
            LOG(INFO, "running Method for Event Delegate!");

            auto pEvents = make_unique<struct epoll_event[]>(backlogEventNum);
            if( pEvents == nullptr ) {
                LOG(ERROR, "malloc error[",errno,"] :" ,strerror_r(errno, errorStr, sizeof(errorStr)));
                throw new system_error();
            }

            while (true)
            {
                int nCnt = epoll_wait(epollFd, pEvents.get(), backlogEventNum, timeout);
                if (nCnt == -1 ) {

                    LOG(ERROR, "Wait error[",errno,"] :" ,strerror_r(errno, errorStr, sizeof(errorStr)));
                    throw new system_error();

                } else {

                    LOG(DEBUG, "timeout!");

                }

                for(int i=0; i<nCnt; i++) {

                    if( pEvents[i].data.fd == listenSocket ) {

                        std::function<int()> function = reinterpret_cast< int(*)() >( pEvents[i].data.ptr );

                        if( -1 == function() ) {
                            LOG(ERROR, "Epoll add socket to failure [",errno,"] :" ,strerror_r(errno, errorStr, sizeof(errorStr)));
                            throw new system_error();
                        }
                    }
                    else {

                        std::function<int(int)> function = reinterpret_cast< int(*)(int) >( pEvents[i].data.ptr );

                        if( -1 == function(pEvents[i].data.fd) ) {
                            LOG(ERROR, "fail to process Data [",errno,"] :" ,strerror_r(errno, errorStr, sizeof(errorStr)));
                        }

                    }
                }
            }


        }

    } // doori
} // CommunicationMember