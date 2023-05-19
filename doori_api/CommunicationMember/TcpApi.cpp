//
// Created by jaeseong on 23. 3. 1.
//

#include <fcntl.h>
#include <functional>
#include "TcpApi.h"

namespace doori {
    namespace CommunicationMember {
        TcpApi::Socket TcpApi::CreateSocket() {

            char errorStr[1024] = {0};
            auto sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                LOG(ERROR, "TCP socket fd, fail to open", strerror_r(errno, errorStr, sizeof(errorStr)));
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            return Socket{sock, SOCK_STATUS::INIT};
        }

        TcpApi::Socket TcpApi::SetReuseOpt(Socket socket, std::string ip, std::string port) {

            if(socket.IsFailure())
                return Socket{-1,SOCK_STATUS::FAILURE};

            uint16_t u16Temp = 0;
            u16Temp = atoi(port.c_str());

            struct sockaddr_in sockaddrIn{};
            sockaddrIn.sin_family = AF_INET;
            sockaddrIn.sin_port = htons(u16Temp);
            sockaddrIn.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());

            if (setsockopt(socket.fd, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, (void *) &sockaddrIn, sizeof(::sockaddr_in)) <
                0) {
                LOG(ERROR, "TCP socket socket, fail to setsockopt");
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            return socket;
        }

        TcpApi::Socket TcpApi::Bind(Socket socket, std::string ip, std::string port) {

            if(socket.IsFailure())
                return Socket{-1,SOCK_STATUS::FAILURE};

            char errorStr[1024] = {0};

            uint16_t u16Temp = 0;
            u16Temp = atoi(port.c_str());

            struct sockaddr_in sockaddrIn{};
            sockaddrIn.sin_family = AF_INET;
            sockaddrIn.sin_port = htons(u16Temp);
            sockaddrIn.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());

            if (bind(socket.fd, (struct sockaddr *) &sockaddrIn, sizeof(::sockaddr_in)) < 0) {
                LOG(ERROR, "TCP socket socket, fail to bind:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            socket.SetBitwise(socket.fd, SOCK_STATUS::BINDING);

            return socket;
        }

        TcpApi::Socket TcpApi::Listen(Socket socket, int backlogNum) {

            if(socket.IsFailure())
                return Socket{-1,SOCK_STATUS::FAILURE};

            if( !socket.IsBitwise(SOCK_STATUS::BINDING) )
                return Socket{-1,SOCK_STATUS::FAILURE};

            char errorStr[1024] = {0};
            if (listen(socket.fd, backlogNum) != 0) {
                LOG(ERROR, "TCP socket bindingSocket, fail to listen:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            socket.SetBitwise(socket.fd, SOCK_STATUS::LISTEN);

            return socket;
        }

        TcpApi::Socket TcpApi::Accept(Socket socket) {

            if(socket.IsFailure())
                return Socket{-1,SOCK_STATUS::FAILURE};

            if( !socket.IsBitwise(SOCK_STATUS::BINDING) )
                return Socket{-1,SOCK_STATUS::FAILURE};

            if( !socket.IsBitwise(SOCK_STATUS::LISTEN) )
                return Socket{-1,SOCK_STATUS::FAILURE};

            int acptFd = 0;
            char errorStr[1024] = {0};

            struct sockaddr_in sockaddrIn{};

            socklen_t len = sizeof(::sockaddr_in);
            // sockaddrIn 변수는 접속한 클라이언트 주소 저장용도로 사용가능하다.
            if ((acptFd = accept(socket.fd, (struct sockaddr *) &sockaddrIn, &len)) == -1) {
                LOG(ERROR, "TCP socket listenSocket, fail to accept:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            socket.SetBitwise(acptFd, SOCK_STATUS::ACCEPT);
            socket.SetBitwise(acptFd, SOCK_STATUS::ESTABLISED);

            return socket;
        }

        TcpApi::Socket TcpApi::Connect(Socket socket, string ip, string port) {

            if(socket.IsFailure())
                return Socket{-1,SOCK_STATUS::FAILURE};

            char errorStr[1024] = {0};

            if (ip.empty() || port.empty()) {
                LOG(FATAL, "ip, port value is empty.");
                std::exit(-1);
            }

            uint16_t u16Temp = 0;
            u16Temp = atoi(port.c_str());

            struct sockaddr_in sockaddrIn{};
            sockaddrIn.sin_family = AF_INET;
            sockaddrIn.sin_port = htons(u16Temp);
            sockaddrIn.sin_addr.s_addr = inet_addr(ip.c_str());

            auto iRet = connect(socket.fd, (struct sockaddr *)&sockaddrIn, sizeof(struct sockaddr_in));
            if (iRet < 0) {
                LOG(ERROR, "TCP socket establishedSocket, fail to connect:", strerror_r(errno, errorStr, sizeof(errorStr)));
                close(socket.fd);
                socket.SetBitwise(socket.fd, SOCK_STATUS::CLOSED);
                socket.SetBitwise(socket.fd, SOCK_STATUS::FAILURE);
            }

            socket.SetBitwise(socket.fd, SOCK_STATUS::ESTABLISED);

            return socket;
        }

        TcpApi::Socket TcpApi::Send(Socket socket, const char *data, uint8_t dataLen) {

            if(socket.IsFailure())
                return Socket{-1,SOCK_STATUS::FAILURE};

            if( !socket.IsBitwise(SOCK_STATUS::ESTABLISED) )
                return Socket{-1,SOCK_STATUS::FAILURE};

            char errorStr[1024] = {0};

            int nRet = 0;
            errno = 0;
            nRet = send(socket.fd, data, dataLen, 0);
            if(nRet == -1)
            {
                LOG(ERROR, "TCP send(), fail to connect:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            LOG(DEBUG, "complete to send data! size[", nRet, "]");

            return socket;
        }

        TcpApi::Socket TcpApi::Recv(Socket socket, char *dataContainer, std::uint8_t dataLen) {

            if(socket.IsFailure())
                return Socket{-1,SOCK_STATUS::FAILURE};

            if( !socket.IsBitwise(SOCK_STATUS::ESTABLISED) )
                return Socket{-1,SOCK_STATUS::FAILURE};

            char errorStr[1024] = {0};

            int nRet = 0;
            errno = 0;
            nRet = recv(socket.fd, dataContainer, dataLen, MSG_WAITALL);
            if(nRet == -1)
            {
                LOG(ERROR, "TCP recv(), fail to connect:", strerror_r(errno, errorStr, sizeof(errorStr)));
                return Socket{-1,SOCK_STATUS::FAILURE};
            }
            LOG(DEBUG, dataContainer);
            LOG(DEBUG, "recv data! size[", nRet, "]");

            return socket;
        }

        TcpApi::Socket TcpApi::SetTimeoutOpt(Socket socket, std::uint8_t timeout) {

            if(socket.IsFailure())
                return Socket{-1,SOCK_STATUS::FAILURE};
            /*
              소켓 설정을 논블로킹으로 셋팅함
              그래야, recv()함수에서 Timeout를 설정할 수 있음
            */
            int flags = fcntl(socket.fd, F_GETFL, 0);  // 소켓의 플래그 값을 가져옴

            fcntl(socket.fd, F_SETFL, flags | O_NONBLOCK);  // 소켓의 플래그 값을 변경하여 넌블로킹 모드로 설정

            struct timeval tv{};
            tv.tv_sec  = timeout;
            tv.tv_usec = 0;

            if (setsockopt(socket.fd, SOL_SOCKET, SO_RCVTIMEO, (void *) &tv, sizeof(struct timeval)) < 0) {
                LOG(ERROR, "TCP socket socket, fail to setsockopt");
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            return socket;
        }

        TcpApi::Socket TcpApi::Connect(string ip, string port, std::uint8_t timeout) {
            char errorStr[1024] = {0};

            auto socket = CreateSocket();
            if (socket.IsFailure())
            {
                LOG(ERROR, "Connect error, at CreateSocket()");
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            socket = SetTimeoutOpt(socket, timeout);
            if(socket.IsFailure())
            {
                LOG(ERROR, "Connect error, at SetTimeoutOpt()");
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            socket = Connect(socket, ip, port);
            if(socket.IsFailure())
            {
                LOG(ERROR, "Connect error, at Connect()");
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            return socket;
        }

        TcpApi::Socket TcpApi::Reply(Socket sockeet, const char *data, uint8_t dataLen, char *recvData, uint8_t tilDataSize) {

            if(sockeet.IsFailure())
                return Socket{-1,SOCK_STATUS::FAILURE};

            if( !sockeet.IsBitwise(SOCK_STATUS::ESTABLISED))
                return Socket{-1,SOCK_STATUS::FAILURE};

            int iRcvDataLen = 0;

            auto socket = Send(sockeet, data, dataLen);
            if( socket.IsFailure() )
            {
                LOG(ERROR, "Reply error, at Send()");
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            socket = Recv(sockeet, recvData, tilDataSize);
            if( socket.IsFailure() )
            {
                LOG(ERROR, "Reply error, at Send()");
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            return socket;
        }

        TcpApi::Socket TcpApi::CreateEpoll(Socket socket, int backlogNum ) {

            if(socket.IsFailure())
                return Socket{-1,SOCK_STATUS::FAILURE};

            if( !socket.IsBitwise(SOCK_STATUS::LISTEN))
                return Socket{-1,SOCK_STATUS::FAILURE};

            auto iEpollFd = epoll_create1(0);
            if( iEpollFd == -1 )
            {
                LOG(ERROR, "Create error" );
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            struct epoll_event ev{};

            ev.events = EPOLLIN ;
            ev.data.fd = socket.fd;
            if (epoll_ctl(iEpollFd, EPOLL_CTL_ADD, socket.fd, &ev) == -1)
            {
                LOG(ERROR, "fail to register listening socket to epoll list" );
                return Socket{-1,SOCK_STATUS::FAILURE};
            }

            return Socket{iEpollFd, SOCK_STATUS::INIT};
        }

        auto TcpApi::RunningEpoll(Socket epoll, Socket socket, int backlogEventNum, int timeout, int(*delegation)(Socket)) -> void {

            if(epoll.IsFailure())
                throw system_error();

            if(socket.IsFailure())
                throw system_error();

            if( !socket.IsBitwise(SOCK_STATUS::LISTEN) )
                throw system_error();

            char	errorStr[1024]={0};

            LOG(INFO, "==================================");
            LOG(INFO, "running Method for Event Delegate!");

            auto pEvents = make_unique<struct epoll_event[]>(backlogEventNum);
            if( pEvents == nullptr ) {
                LOG(ERROR, "malloc error[",errno,"] :" ,strerror_r(errno, errorStr, sizeof(errorStr)));
                throw system_error();
            }

            while (true)
            {
                errno = 0;
                int nCnt = epoll_wait(epoll.fd, pEvents.get(), backlogEventNum, timeout);
                if (nCnt == -1 ) {

                    LOG(ERROR, "Wait error[",errno,"] :" ,strerror_r(errno, errorStr, sizeof(errorStr)));
                    throw system_error();

                } else if (nCnt == 0){

                    LOG(DEBUG, "timeout!");
                    continue;

                }

                for(int i=0; i<nCnt; i++) {

                    if(pEvents[i].data.fd == socket.fd ) {

                        LOG(DEBUG, "Listen -> Accept");

                        if( AddAsEpollList(epoll, socket, delegation).IsFailure() ) {
                            LOG(ERROR, "Epoll add socket to failure [",errno,"] :" ,strerror_r(errno, errorStr, sizeof(errorStr)));
                            throw system_error();
                        }
                    }
                    else {

                        LOG(DEBUG, "Recv -> Processing");

                        int(*pFunc)(int) = (  int(*)(int)  )( pEvents[i].data.ptr );
                        LOG(INFO, "Recv FD[", pEvents[i].data.fd, "]");

                        if( -1 == pFunc(pEvents[i].data.fd) ) {
                            LOG(ERROR, "fail to process Data [",errno,"] :" ,strerror_r(errno, errorStr, sizeof(errorStr)));
                        }

                    }
                }
            }


        }

        TcpApi::Socket TcpApi::AddAsEpollList(Socket epoll, Socket socket, int(*delegation)(Socket) ) {

            auto conn_sock = CommunicationMember::TcpApi::Accept(socket);
            if (conn_sock.IsFailure()) {
                LOG(ERROR, "fail to accept socket.");
                return Socket{-1, SOCK_STATUS::FAILURE};
            }

            LOG(INFO, "Accepted FD[", conn_sock.fd,"]");

            // Timeout 설정함
            auto socket = CommunicationMember::TcpApi::SetTimeoutOpt(conn_sock, 10);
            if (socket.IsFailure()) {
                LOG(ERROR, "fail to SetTimeoutOpt.");
                return Socket{-1, SOCK_STATUS::FAILURE};
            }

            struct epoll_event ev{};
            ev.events = EPOLLIN;
            ev.data.fd = socket.fd;
            ev.data.ptr = (void*)delegation;
            if (epoll_ctl(epoll.fd, EPOLL_CTL_ADD, socket.fd, &ev) == -1) {
                LOG(ERROR, "epoll, fail to EPOLL_CTL_ADD");
                return Socket{-1, SOCK_STATUS::FAILURE};
            }

            return epoll;
        }

    } // doori
} // CommunicationMember