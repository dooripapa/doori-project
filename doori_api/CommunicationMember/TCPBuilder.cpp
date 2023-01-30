//
// Created by jaeseong on 23. 1. 31.
//

#include "TCPBuilder.h"
#include "TCP.h"
#include "Common/Log.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

namespace doori::CommunicationMember {
    IIPCBuilder::Type TCPBuilder::GetType() {
        return Type::TCP;
    }

    TCPBuilder::TCPBuilder(Endpoint From, Endpoint To) : mFrom{From}, mTo{To} {
    }

    unique_ptr<IConnection> TCPBuilder::GetConnection() {
        return make_unique<IConnection>( new TCP{m_From.Address().Ip(), m_From.Address().Port(), m_To.Address().Ip(), m_To.Address().Port()} );
    }

    /**
     * 초기화된 struct sockaddr_in 구조체를 이용하여, 바인딩된(재사용)소켓을 리턴.
     * @param addr
     * @return
     */
    auto TCPBuilder::Bind(struct sockaddr_in addr) -> int {
        int socketFd = 0;
        char errorStr[1024] = {0};
        socketFd = socket(addr.sin_family, SOCK_STREAM, 0);
        if (socketFd < 0) {
            LOG(ERROR, "TCP socket fd, fail to open");
            return -1;
        }

        if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, (void *) &addr, sizeof(struct sockaddr_in)) < 0) {
            LOG(ERROR, "TCP socket fd, fail to setsockopt");
            return -1;
        }

        if (bind(socketFd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) < 0) {
            LOG(ERROR, "TCP socket fd, fail to bind:", strerror_r(errno, errorStr, sizeof(errorStr)));
            return -1;
        }
        return socketFd;
    }

    /**
     * IP, Port 문자열을 입력 받아, 소켓 생성을 위한 저수준 struct sockaddr_in 초기화
     * @note ip값이 "" 인 경우 바인딩은 INADDR_ANY로 셋팅됨
     * @param ip
     * @param port
     * @return
     */
    auto TCPBuilder::Init(string ip, string port) -> struct sockaddr_in {
        struct sockaddr_in sockaddrIn;

        memset(&sockaddrIn, 0x00, sizeof(struct sockaddr_in));

        uint16_t u16Temp = 0;
        u16Temp = atoi(port.c_str());

        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(u16Temp);
        sockaddrIn.sin_addr.s_addr = ip.empty() ? INADDR_ANY : inet_addr(ip.c_str());

        return sockaddrIn;
    }

    /**
     * 입력 받은 소켓을 listen상태로 변경.
     * @param socketFd
     * @return
     */
    auto TCPBuilder::Listen(int socketFd) -> bool{
        char errorStr[1024] = {0,};
        if (listen(socketFd, 1) < 0) {
            LOG(ERROR, "TCP socket fd, fail to listen:", strerror_r(errno, errorStr, sizeof(errorStr)));
            return false;
        }
        return true;
    }

    /**
     * 입력받은 소켓과 accept의 source address로 accept 상태로 변경
     * @param socketFd
     * @param sourceAddr
     * @return
     */
    auto TCPBuilder::Accept(int socketFd, struct sockaddr_in sourceAddr) -> bool{
        char errorStr[1024] = {0,};
        auto len = sizeof(struct sockaddr_in);
        if (accept(socketFd, (struct sockaddr *)&sourceAddr, &len) < 0) {
            LOG(ERROR, "TCP socket fd, fail to accept:", strerror_r(errno, errorStr, sizeof(errorStr)));
            return false;
        }
        return true;
    }
} // doori