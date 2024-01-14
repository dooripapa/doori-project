//
// Created by jslee on 24. 1. 6.
//

#include <string>
#include <functional>
#include "TCPNode.h"

namespace doori::api::Communication::TCP {

    void TCPNode::tieSource(const string &ip, const string &port, TCPNode::dispatcherReceiver) noexcept {

        _source.ip = ip;
        _source.port = port;

    }

    void TCPNode::tieRemote(const string &ip, const string &port) noexcept {

        _remote.ip = ip;
        _remote.port = port;

    }

    int TCPNode::setState(std::unique_ptr <ITCPState> state) noexcept {
        _state = std::move(state);
        return 0;
    }

    Address TCPNode::getSource() {
        return _source;
    }

    Address TCPNode::getRemote() {
        return _remote;
    }

} // doori