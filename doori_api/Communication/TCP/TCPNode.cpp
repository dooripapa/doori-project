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

    void TCPNode::changeState(ITCPState *state) {
        _state = state;
    }

    void TCPNode::setSock(int fd) noexcept {
        _sock = fd;
    }

    void TCPNode::open() {
        _state->open();
    }

    void TCPNode::wait() {
        _state->wait();
    }

    void TCPNode::establish() {
        _state->establish();
    }

    void TCPNode::close() {
        _state->close();
    }


} // doori