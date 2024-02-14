//
// Created by jslee on 24. 1. 6.
//

#ifndef DOORI_PROJECT_TCPNODE_H
#define DOORI_PROJECT_TCPNODE_H

#include <memory>
#include <sys/socket.h>
#include "Common/Error.h"

using namespace std;

namespace doori::api::Communication::TCP {

    class TCPState;

    struct Address {
        string ip;
        string port;
    };

    class TCPNode : public doori::api::Common::Error{

    public:

        explicit TCPNode();

        void tieSource(const string &ip, const string &port) noexcept;
        void tieRemote(const string &ip, const string &port) noexcept;

        Address getSource();

        Address getRemote();

        [[nodiscard]] int getSock() const noexcept;

        void setSock(int fd) noexcept;

        long transmit(const string &data) const;

        long transmit(const char *data, uint16_t dataSize) const;

        template<int N>
        long transmit(char const(&data)[N]) const {
            auto ret = send(_sock, data, N - 1, 0);
            if (ret == -1) {
                LOG(ERROR, "send error");
                return -1;
            }
            return ret;
        };

        long receive(string &container, uint32_t tilDataSize) const;

        /**
         * 상태패턴 관계함수.
         * 노드가 소켓 상태를 가짐.
         */
        void establish();

        void close();

        void listen();

    private:

        /**
         * c 기반 const char* 형식의 데이터 송신함수
         * @param data
         * @param dataSize
         * @return
         */
        long cSend(const char *data, uint16_t dataSize) const;

        friend class TCPState;

        void changeState(TCPState *state);

    private:

        TCPState *_state;

        int _sock = -1;

        Address _source;
        Address _remote;
    };

} // doori

#endif //DOORI_PROJECT_TCPNODE_H