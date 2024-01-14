//
// Created by jslee on 24. 1. 6.
//

#ifndef DOORI_PROJECT_TCPNODE_H
#define DOORI_PROJECT_TCPNODE_H

#include <memory>
#include "ITCPState.h"

using namespace doori::api::Communication;
using namespace std;

namespace doori::api::Communication::TCP {

    struct Address {
        string ip;
        string port;
    };

    class TCPNode {

        using dispatcherReceiver = function<int(int fd, const string &buffer)>;
        using dispatcherSender = function<int(const string &buffer, int bufferSize)>;

    public:

        void tieSource(const string &ip, const string &port, dispatcherReceiver) noexcept;

        void tieRemote(const string &ip, const string &port) noexcept;

        int setState(unique_ptr<ITCPState> state) noexcept;

        Address getSource();
        Address getRemote();

    private:

        std::unique_ptr<ITCPState> _state = nullptr;

        int _sock;

        Address _source;
        Address _remote;
    };
} // doori

#endif //DOORI_PROJECT_TCPNODE_H
