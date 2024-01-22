//
// Created by jslee on 24. 1. 6.
//

#include <string>
#include <functional>
#include <variant>
#include "TCPNode.h"

namespace doori::api::Communication::TCP {

    void TCPNode::tieSource(const string &ip, const string &port) noexcept {
        _source.ip = ip;
        _source.port = port;
    }

    void TCPNode::tieRemote(const string &ip, const string &port) noexcept {
        _remote.ip = ip;
        _remote.port = port;
    }

    Address TCPNode::getSource() {
        return _source;
    }

    Address TCPNode::getRemote() {
        return _remote;
    }

    int TCPNode::getSock() const noexcept {
        return _sock;
    }

    void TCPNode::setSock(int fd) noexcept {
        _sock = fd;
    }

    void TCPNode::wait() {
        _state->wait(*this);
    }

    void TCPNode::establish() {
        _state->establish(*this);
    }

    void TCPNode::close() {
        _state->close(*this);
    }

    long TCPNode::cSend(const char *data, uint16_t dataSize) const {

        char acTemp[1024] = {0,};

        auto ret = send(_sock, data, dataSize, 0);
        if (ret == -1) {
            auto p = strerror_r(errno, acTemp, sizeof(acTemp) - 1);
            LOG(ERROR, "send errno:", errno, " cause:", p);
            return -1;
        }
        return ret;
    }

    long TCPNode::transmit(const string &data) const {
        LOG(DEBUG, "Send() stream:", data);

        return TCPNode::cSend(data.c_str(), data.length());
    }

    long TCPNode::transmit(const char *data, uint16_t dataSize) const {

        char acTemp[1024] = {0,};

        auto ret = send(_sock, data, dataSize, 0);
        if(ret == -1) {
            auto p = strerror_r(errno, acTemp, sizeof(acTemp) - 1);
            LOG(ERROR, "send errno:", errno, " cause:", p);
            return -1;
        }
        return ret;
    }

    long TCPNode::receive(string &container, uint32_t tilDataSize) const {

        char acTemp[1024] = {0,};

        // 1 null space
        auto p = make_unique<char[]>(tilDataSize+1);
        if(p == nullptr) {
            assert("malloc error");
        }

        auto ret = recv(_sock, p.get(), tilDataSize, MSG_WAITALL);
        if(ret == -1) {
            auto p = strerror_r(errno, acTemp, sizeof(acTemp) - 1);
            LOG(ERROR, "recv errno:", errno, " cause:", p);
            return -1;
        }

        container = p.get();

        return ret;
    }

    void TCPNode::setState(std::unique_ptr<ITCPState> state) {

        _state = std::move(state);

    }

} // doori