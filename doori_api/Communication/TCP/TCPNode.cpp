//
// Created by jslee on 24. 1. 6.
//

#include <string>
#include <functional>
#include "TCPNode.h"

namespace doori::api::Communication::TCP {
    void TCPNode::tieSource(const string &ip, const string &port,
                            std::function<int(const string &)> dispatcherReceiver) noexcept {
        _sourceIp = ip;
        _sourcePort = port;
}

    void TCPNode::tieRemote(const string &ip, const string &port,
                            function<int(const string &buffer, int bufferSize)> dispatcherSender) noexcept {
        _remoteIp = ip;
        _remotePort = port;
    }
} // doori