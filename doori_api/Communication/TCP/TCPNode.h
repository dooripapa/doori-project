//
// Created by jslee on 24. 1. 6.
//

#ifndef DOORI_PROJECT_TCPNODE_H
#define DOORI_PROJECT_TCPNODE_H

using namespace doori::api::Communication;
using namespace std;

namespace doori::api::Communication::TCP {
    class TCPNode {

        using dispatcherReceiver = function<int(int fd, const string &buffer)>;
        using dispatcherSender = function<int(const string &buffer, int bufferSize)>;

    public:

        void tieSource(const string &ip, const string &port, dispatcherReceiver) noexcept;

        void tieRemote(const string &ip, const string &port, dispatcherSender) noexcept;

    private:

        int _sock;

        string _sourceIp;
        string _sourcePort;
        string _remoteIp;
        string _remotePort;
    };
} // doori

#endif //DOORI_PROJECT_TCPNODE_H
