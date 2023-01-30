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
        Type GetType() override;

        TCPBuilder() = delete;
        TCPBuilder(Endpoint From, Endpoint To);

    private:
        unique_ptr<IConnection> GetConnection() override;

        static auto Init(string ip, string port) -> struct sockaddr_in;
        static auto Bind(struct sockaddr_in addr) -> int;
        static auto Listen(int socketFd) -> bool;
        static auto Accept(int socketFd, struct sockaddr_in addr) -> bool;
    };

} // doori

#endif //DOORI_PROJECT_TCPBUILDER_H
