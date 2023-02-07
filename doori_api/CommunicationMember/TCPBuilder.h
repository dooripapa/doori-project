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
        static auto Init(string ip, string port) -> struct sockaddr_in;
        static auto Bind(struct sockaddr_in addr) -> int;
        static auto Listen(int socketFd) -> bool;
        static auto Accept(int socketFd, struct sockaddr_in addr) -> bool;

        Endpoint mFrom;
        Endpoint mTo;
    };

} // doori

#endif //DOORI_PROJECT_TCPBUILDER_H
