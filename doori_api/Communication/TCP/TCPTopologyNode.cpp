//
// Created by jslee on 24. 1. 6.
//

#include <string>
#include "TCPTopologyNode.h"

namespace doori::api::Communication::TCP {
    long TCPTopologyNode::Send(const std::string &data) const {
        return 0;
    }

    long TCPTopologyNode::Recv(std::string &data, std::uint32_t utilSize) const {
        return 0;
    }

    int TCPTopologyNode::Close() {
        return 0;
    }
} // doori