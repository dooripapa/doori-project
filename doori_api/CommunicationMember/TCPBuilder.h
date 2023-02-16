//
// Created by jaeseong on 23. 1. 31.
//

#ifndef DOORI_PROJECT_TCPBUILDER_H
#define DOORI_PROJECT_TCPBUILDER_H

#include "CommunicationMember/IIPCBuilder.h"
#include "Endpoint.h"
#include <string>

using namespace std;

namespace doori::CommunicationMember {

    class TCPBuilder : IIPCBuilder {
    public:

        TCPBuilder() = delete;
        TCPBuilder(Endpoint From, Endpoint To);

    private:
        int InitFrom() override;

        int InitTo() override;

    private:
        /**
         * ip, port 입력 받아서, struct sockaddr_in 를 초기화합니다.
         * @param ip
         * @param port
         * @return 초기화된 struct sockaddr_in
         */
        static auto Init(string ip, string port) -> struct sockaddr_in;

        /**
         * 배정이 완료된 struct sockaddr_in 이용하여, 바인딩 완료된 socket fd 리턴함
         * @param addr struct sockaddr_in
         * @return 바인딩이 된 socket fd
         */
        static auto Bind(struct sockaddr_in addr) -> int;
        static auto Listen(int socketFd) -> bool;
        static auto Accept(int socketFd, struct sockaddr_in addr) -> bool;

        Endpoint mFrom;
        Endpoint mTo;
    };

} // doori

#endif //DOORI_PROJECT_TCPBUILDER_H
