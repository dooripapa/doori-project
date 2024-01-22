//
// Created by jslee on 24. 1. 6.
//

#ifndef DOORI_PROJECT_TCPNODE_H
#define DOORI_PROJECT_TCPNODE_H

#include <memory>
#include "TCPClose.h"
#include "TCPEstablish.h"
#include "TCPWait.h"
#include "../../Common/Log.h"
#include "../../Common/Error.h"
#include <sys/socket.h>
#include <bits/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <cassert>


using namespace doori::api::Communication;
using namespace std;

namespace doori::api::Communication::TCP {

    class ITCPState;

    struct Address {
        string ip;
        string port;
    };

    class TCPNode {

    public:

        void tieSource(const string &ip, const string &port) noexcept;

        void tieRemote(const string &ip, const string &port) noexcept;

        Address getSource();
        Address getRemote();

        void setState(std::unique_ptr<ITCPState> state);

        [[nodiscard]] int getSock() const noexcept;

        void setSock(int fd) noexcept;

        void establish();

        void close();

        void wait();

        long transmit(const string& data) const ;

        long transmit(const char* data, uint16_t dataSize) const;

        template<int N>
        long transmit(char const(&data)[N]) const {
            auto ret = send(_sock, data, N-1, 0);
            if(ret == -1) {
                LOG(ERROR, "send error" );
                return -1;
            }
            return ret;
        };

        long receive(string &container, uint32_t tilDataSize) const;

    private:

        long cSend(const char *data, uint16_t dataSize) const;

        friend class ITCPState;
        void changeState(ITCPState *state);

    private:

        unique_ptr<ITCPState> _state = make_unique<ITCPState>(new TCPClose());

        int _sock = -1;

        Address _source;
        Address _remote;
    };

} // doori

#endif //DOORI_PROJECT_TCPNODE_H
