//
// Created by jaeseong on 23. 1. 31.
//

#ifndef DOORI_PROJECT_TCPBUILDER_H
#define DOORI_PROJECT_TCPBUILDER_H

#include "Communication/IIPCBuilder.h"
#include "Communication/TcpApi.h"
#include "Socket.h"
#include <string>

using namespace std;

namespace doori::Communication{

    class TCPBuilder : public IIPCBuilder {
    public:

        TCPBuilder() = delete;
        explicit TCPBuilder(Socket &socket);
        TCPBuilder(const TCPBuilder&) = delete;
        TCPBuilder(TCPBuilder&&) = delete;
        TCPBuilder& operator=(const TCPBuilder&) = delete;
        TCPBuilder& operator=(TCPBuilder&&) = delete;

        virtual ~TCPBuilder();

        unique_ptr<IIPCTopology> GetTopology() override;

        [[maybe_unused]] void From(string ip, string port);

        [[maybe_unused]] void To(string ip, string port);

        int Establish() override;

    private:
        Socket mEstablishSocket;
        TcpApi mTcpApi;
        string mBindingIp;
        string mBindingPort;
        string mRemoteIp;
        string mRemotePort;
        bool mServer;
        bool mClient;

        int SetServer(string ip, string port, int backlogNum, int timeout);

        int BindingClient(string bindingIp, string bindingPort, string remoteIp, string remotePort, int timeout);
    };

} // doori

#endif //DOORI_PROJECT_TCPBUILDER_H
