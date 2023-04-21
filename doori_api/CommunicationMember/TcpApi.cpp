//
// Created by jaeseong on 23. 3. 1.
//

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

        int TcpApi::Send(int fd, char *data, uint8_t dataLen) {
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
    } // doori
} // CommunicationMember