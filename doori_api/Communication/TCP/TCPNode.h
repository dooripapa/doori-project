//
// Created by jslee on 24. 1. 6.
//

#ifndef DOORI_PROJECT_TCPNODE_H
#define DOORI_PROJECT_TCPNODE_H

#include <memory>
#include "ITCPState.h"
#include "TCPClose.h"
#include "TCPEstablish.h"
#include "TCPOpen.h"
#include "../../Common/Error.h"

using namespace doori::api::Communication;
using namespace std;

namespace doori::api::Communication::TCP {

    struct Address {
        string ip;
        string port;
    };

    class TCPNode : public Common::Error {


    public:

        void tieSource(const string &ip, const string &port) noexcept;

        void tieRemote(const string &ip, const string &port) noexcept;

        Address getSource();
        Address getRemote();

        [[nodiscard]] int getSock() const noexcept;

        void setSock(int fd) noexcept;

        void open();

        void establish();

        void close();

        void wait();

    private:

        ITCPState *_state = new TCPClose();

        int _sock = -1;

        Address _source;
        Address _remote;
    };
} // doori

#endif //DOORI_PROJECT_TCPNODE_H
